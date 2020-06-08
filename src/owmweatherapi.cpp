/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "owmweatherapi.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimeZone>
#include <QUrlQuery>
#include <utility>

OWMWeatherAPI::OWMWeatherAPI(QString locationId, QString timeZone, double latitude, double longitude)
    : AbstractWeatherAPI(std::move(locationId), std::move(timeZone), 3, latitude, longitude)
{
}

OWMWeatherAPI::~OWMWeatherAPI() = default;

void OWMWeatherAPI::applySunriseDataToForecast()
{
    currentData_.setSunrise(currentSunriseData_);
    return;
}

void OWMWeatherAPI::parse(QNetworkReply *reply)
{
    reply->deleteLater();
    if (reply->error()) {
        qDebug() << "network error when fetching forecast:" << reply->errorString();
        emit networkError();
        return;
    }

    /*~~~~~~~~~ static variable ~~~~~~~~*/
    // rank weather (for what best describes the day overall)
    static const QHash<QString, int> rank = {
        // only need neutral icons

        {"weather-clear", 0},
        {"weather-clear-night", 0},
        {"weather-few-clouds", 1},
        {"weather-few-clouds-night", 1},
        {"weather-clouds", 2},
        {"weather-clouds-night", 2},
        {"weather-mist", 2},
        {"weather-many-clouds", 3},
        {"weather-showers", 4},
        {"weather-showers-day", 4},
        {"weather-showers-night", 4},
        {"weather-showers-scattered-day", 5},
        {"weather-showers-scattered-night", 5},
        {"weather-snow", 5},
        {"weather-snow-scattered-day", 5},
        {"weather-snow-scattered-night", 5},
        {"weather-storm", 6},
        {"weather-storm-day", 6},
        {"weather-storm-night", 6},
    };

    /*~~~~~~~~~~~ end of static variable ~~~~~~~~~~*/

    QJsonDocument mJson = QJsonDocument::fromJson(reply->readAll());
    if (mJson["cod"].toInt() == 401) // API Token invalid
    {
        emit TokenInvalid();
        return;
    }
    if (mJson["cod"].toInt() == 429) // calls reached limit
    {
        emit TooManyCalls();
        return;
    }
    AbstractHourlyWeatherForecast hourly;
    QList<AbstractHourlyWeatherForecast> hourlyList;
    QHash<QDate, AbstractDailyWeatherForecast> dayCache;

    int offset = mJson["city"].toObject()["timezone"].toInt();
    QJsonArray mArray = mJson["list"].toArray();
    for (auto fc : mArray) {
        auto date = QDateTime::fromSecsSinceEpoch(fc.toObject()["dt"].toInt()).toTimeZone(QTimeZone(offset));
        hourly = AbstractHourlyWeatherForecast();
        hourly.setDate(date);
        hourly.setFog(-1);
        hourly.setUvIndex(-1);
        hourly.setHumidity(fc.toObject()["main"].toObject()["humidity"].toInt());
        hourly.setPressure(fc.toObject()["main"].toObject()["pressure"].toInt());
        hourly.setWindSpeed(fc.toObject()["wind"].toObject()["speed"].toDouble());
        hourly.setTemperature(fc.toObject()["main"].toObject()["temp"].toDouble());
        hourly.setNeutralWeatherIcon(neutralApiDescMap[fc.toObject()["weather"].toArray().at(0)["icon"].toString()].icon);
        hourly.setWeatherIcon(apiDescMap[fc.toObject()["weather"].toArray().at(0)["icon"].toString()].icon);
        hourly.setWindDirection(getWindDirect(fc.toObject()["wind"].toObject()["deg"].toDouble()));
        hourly.setWeatherDescription(apiDescMap[fc.toObject()["weather"].toArray().at(0)["icon"].toString()].desc /*fc.toObject()["weather"].toArray().at(0)["description"].toString()*/);
        hourly.setPrecipitationAmount(fc.toObject()["rain"].toObject()["3h"].toDouble() + fc.toObject()["snow"].toObject()["3h"].toDouble());
        hourlyList.push_back(hourly);
        // add day if not already created
        if (!dayCache.contains(date.date())) {
            dayCache[date.date()] = AbstractDailyWeatherForecast(-1e9, 1e9, 0, 0, 0, 0, "weather-none-available", "", date.date());
        }

        // update day forecast with hour information if needed
        AbstractDailyWeatherForecast &dayForecast = dayCache[date.date()];

        dayForecast.setPrecipitation(dayForecast.precipitation() + hourly.precipitationAmount());
        dayForecast.setUvIndex(std::max(dayForecast.uvIndex(), hourly.uvIndex()));
        dayForecast.setHumidity(std::max(dayForecast.humidity(), hourly.humidity()));
        dayForecast.setPressure(std::max(dayForecast.pressure(), hourly.pressure()));

        dayForecast.setMaxTemp(std::max(dayForecast.maxTemp(), (float)fc.toObject()["main"].toObject()["temp_max"].toDouble()));
        dayForecast.setMinTemp(std::min(dayForecast.minTemp(), (float)fc.toObject()["main"].toObject()["temp_min"].toDouble()));

        // set description and icon if it is higher ranked
        if (rank[hourly.weatherIcon()] >= rank[dayForecast.weatherIcon()]) {
            dayForecast.setWeatherDescription(hourly.weatherDescription());
            dayForecast.setWeatherIcon(hourly.neutralWeatherIcon());
        }
    }

    auto forecasts = AbstractWeatherForecast(QDateTime::currentDateTime(), locationId_, latitude_, longitude_, hourlyList, dayCache.values());
    currentData_ = forecasts;
    emit updated(forecasts);
}

void OWMWeatherAPI::update()
{
    // don't update if updated recently, and forecast is not empty
    if (!currentData_.dailyForecasts().empty() && !currentData_.hourlyForecasts().empty() &&
        currentData_.timeCreated().secsTo(QDateTime::currentDateTime()) < 300) {
        emit updated(currentData_);
        return;
    }

    QUrlQuery query;
    QSettings settings;
    query.addQueryItem(QLatin1String("lat"), QString().setNum(latitude_));
    query.addQueryItem(QLatin1String("lon"), QString().setNum(longitude_));
    query.addQueryItem(QLatin1String("APPID"), settings.value("Global/OWMToken").toString());
    query.addQueryItem(QLatin1String("units"), QLatin1String("metric"));

    QUrl url;
    url.setScheme(QLatin1String("http"));
    url.setHost(QLatin1String("api.openweathermap.org"));
    url.setPath(QLatin1String("/data/2.5/forecast"));
    url.setQuery(query);
    qDebug() << url;

    QNetworkRequest req(url);
    mReply = mManager->get(req);
    connect(mManager, &QNetworkAccessManager::finished, this, &OWMWeatherAPI::parse);
}
