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

OWMWeatherAPI::OWMWeatherAPI(QString locationName)
    : AbstractWeatherAPI(locationName, 3)
{
}

OWMWeatherAPI::~OWMWeatherAPI()
{
}

void OWMWeatherAPI::setLocation(float latitude, float longitude)
{
    lat = latitude;
    lon = longitude;
}

void OWMWeatherAPI::setToken(QString &token)
{
    token_ = &token;
}

void OWMWeatherAPI::parse(QNetworkReply *reply)
{
    QJsonDocument mJson = QJsonDocument::fromJson(reply->readAll());
    auto forecasts = new AbstractWeatherForecast();
    AbstractHourlyWeatherForecast *hourly;
    QList<AbstractHourlyWeatherForecast *> hourlyList;
    int offset = mJson["city"].toObject()["timezone"].toInt();
    QJsonArray mArray = mJson["list"].toArray();
    for (auto fc : mArray) {
        auto date = QDateTime::fromSecsSinceEpoch(fc.toObject()["dt"].toInt()).toTimeZone(QTimeZone(offset));
        hourly = new AbstractHourlyWeatherForecast();
        hourly->setDate(date);
        hourly->setFog(-1);
        hourly->setUvIndex(-1);
        hourly->setHumidity(fc.toObject()["main"].toObject()["humidity"].toInt());
        hourly->setPressure(fc.toObject()["main"].toObject()["pressure"].toInt());
        hourly->setWindSpeed(fc.toObject()["wind"].toObject()["speed"].toDouble());
        hourly->setTemperature(fc.toObject()["main"].toObject()["temp"].toDouble());
        hourly->setWeatherIcon(map[fc.toObject()["weather"].toArray().at(0)["icon"].toString()]);
        hourly->setWindDirection(getWindDirect(fc.toObject()["wind"].toObject()["deg"].toDouble()));
        hourly->setWeatherDescription(fc.toObject()["weather"].toArray().at(0)["description"].toString());
        hourly->setPrecipitationAmount(fc.toObject()["rain"].toObject()["3h"].toDouble() + fc.toObject()["snow"].toObject()["3h"].toDouble());
        hourlyList.push_back(hourly);
        // add day if not already created
        if (!dayCache.contains(date.date())) {
            dayCache[date.date()] = new AbstractDailyWeatherForecast(-1e9, 1e9, 0, 0, 0, 0, "weather-none-available", "", date.date());
        }

        // update day forecast with hour information if needed
        AbstractDailyWeatherForecast *dayForecast = dayCache[date.date()];

        dayForecast->setPrecipitation(dayForecast->precipitation() + hourly->precipitationAmount());
        dayForecast->setUvIndex(std::max(dayForecast->uvIndex(), hourly->uvIndex()));
        dayForecast->setHumidity(std::max(dayForecast->humidity(), hourly->humidity()));
        dayForecast->setPressure(std::max(dayForecast->pressure(), hourly->pressure()));

        dayForecast->setMaxTemp(std::max(dayForecast->maxTemp(), (float)fc.toObject()["main"].toObject()["temp_max"].toDouble()));
        dayForecast->setMinTemp(std::min(dayForecast->minTemp(), (float)fc.toObject()["main"].toObject()["temp_min"].toDouble()));

        // rank weather (for what best describes the day overall)
        QHash<QString, int> rank = {
            // only need neutral icons

            {"weather-clear", 0},
            {"weather-clear-night", 0},
            {"weather-few-clouds", 1},
            {"weather-clouds", 2},
            {"weather-clouds-night", 2},
            {"weather-mist", 2},
            {"weather-many-clouds", 3},
            {"weather-showers-day", 4},
            {"weather-snowers-night", 4},
            {"weather-showers-scattered-day", 5},
            {"weather-showers-scattered-night", 5},
            {"weather-snow-scattered-day", 5},
            {"weather-snow-scattered-night", 5},
            {"weather-storm-day", 6},
            {"weather-storm-night", 6},
        };

        // set description and icon if it is higher ranked
        if (rank[hourly->weatherIcon()] >= rank[dayForecast->weatherIcon()]) {
            dayForecast->setWeatherDescription(hourly->weatherDescription());
            dayForecast->setWeatherIcon(hourly->weatherIcon());
        }
    }
    forecasts->sortDailyForecast();
    emit updated(forecasts);
}

void OWMWeatherAPI::update()
{
    delete currentData_;
    // delete old data
    QUrlQuery query;

    query.addQueryItem(QStringLiteral("lat"), QString().setNum(lat));
    query.addQueryItem(QStringLiteral("lgn"), QString().setNum(lon));
    query.addQueryItem(QStringLiteral("APPID"), *token_);

    QUrl url;

    url.setScheme(QStringLiteral("http"));
    url.setHost(QStringLiteral("api.openweathermap.org"));
    url.setPath(QStringLiteral("/forecast"));

    QNetworkRequest req(url);
    mReply = mManager->get(req);
    connect(mManager, &QNetworkAccessManager::finished, this, &OWMWeatherAPI::parse);
}
