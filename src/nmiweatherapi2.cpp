/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "nmiweatherapi2.h"
#include "abstractdailyweatherforecast.h"
#include "abstracthourlyweatherforecast.h"
#include "abstractweatherforecast.h"
#include "global.h"

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimeZone>
#include <QUrlQuery>
#include <utility>

NMIWeatherAPI2::NMIWeatherAPI2(QString locationId, QString timeZone, double latitude, double longitude)
    : AbstractWeatherAPI(std::move(locationId), std::move(timeZone), -1, latitude, longitude)
{
}

QString NMIWeatherAPI2::getSymbolCodeDescription(bool isDay, const QString &symbolCode)
{
    return isDay ? apiDescMap[symbolCode + "_day"].desc : apiDescMap[symbolCode + "_night"].desc;
}

QString NMIWeatherAPI2::getSymbolCodeIcon(bool isDay, const QString &symbolCode)
{
    return isDay ? apiDescMap[symbolCode + "_day"].icon : apiDescMap[symbolCode + "_night"].icon;
}

void NMIWeatherAPI2::applySunriseDataToForecast()
{
    currentData_.setSunrise(currentSunriseData_);
    for (int i = 0; i < currentData_.hourlyForecasts().count(); i++) {
        auto hourForecast = currentData_.hourlyForecasts()[i];

        bool isDay;
        if (currentSunriseData_.count() != 0) { // if we have sunrise data
            isDay = sunriseApi_->isDayTime(hourForecast.date());
        } else {
            isDay = hourForecast.date().time().hour() < 7 || hourForecast.date().time().hour() >= 18; // 6:00 - 18:00 is day
        }

        hourForecast.setWeatherIcon(getSymbolCodeIcon(isDay, hourForecast.symbolCode())); // set day/night icon
        hourForecast.setWeatherDescription(getSymbolCodeDescription(isDay, hourForecast.symbolCode()));

        currentData_.hourlyForecasts()[i] = hourForecast;
    }
}

void NMIWeatherAPI2::update()
{
    // don't update if updated recently, and forecast is not empty
    if (!currentData_.dailyForecasts().empty() && !currentData_.hourlyForecasts().empty() && currentData_.timeCreated().secsTo(QDateTime::currentDateTime()) < 300) {
        emit updated(currentData_);
        return;
    }

    // query weather api
    QUrl url("https://api.met.no/weatherapi/locationforecast/2.0/complete");
    QUrlQuery query;
    query.addQueryItem("lat", QString::number(latitude_));
    query.addQueryItem("lon", QString::number(longitude_));

    url.setQuery(query);

    qDebug() << url;
    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    // see §Identification on https://api.met.no/conditions_service.html
    req.setHeader(QNetworkRequest::UserAgentHeader, QString(QCoreApplication::applicationName() + QLatin1Char(' ') + QCoreApplication::applicationVersion() + QLatin1String(" (kde-pim@kde.org)")));

    // TODO see §Cache on https://api.met.no/conditions_service.html
    // see §Compression on https://api.met.no/conditions_service.html
    //    req.setRawHeader("Accept-Encoding", "gzip, deflate");
    mReply = mManager->get(req);
    connect(
        mReply, &QNetworkReply::finished, this, [this]() { this->parse(this->mReply); }, Qt::ConnectionType::UniqueConnection);
}

void NMIWeatherAPI2::parse(QNetworkReply *reply)
{
    reply->deleteLater();
    if (reply->error()) {
        qDebug() << "network error when fetching forecast:" << reply->errorString();
        emit networkError();
        return;
    }

    qDebug() << "data arrived";
    // parse json for weather forecast
    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());

    if (jsonDocument.isObject()) {
        QJsonObject obj = jsonDocument.object();
        QJsonObject prop = obj["properties"].toObject();

        if (prop.contains("timeseries")) {
            QJsonArray timeseries = prop["timeseries"].toArray();

            QHash<QDate, AbstractDailyWeatherForecast> dayCache;
            QList<AbstractHourlyWeatherForecast> hoursList;

            // loop over all forecast data
            for (QJsonValueRef ref : timeseries) {
                QJsonObject refObj = ref.toObject();
                parseOneElement(refObj, dayCache, hoursList);
            }

            // process and build abstract forecast
            currentData_ = AbstractWeatherForecast(QDateTime::currentDateTime(), locationId_, latitude_, longitude_, hoursList, dayCache.values());
        }
    }

    for (auto fc : currentData_.hourlyForecasts()) {
        fc.setDate(fc.date().toTimeZone(QTimeZone(timeZone_.toUtf8())));
    }

    applySunriseDataToForecast(); // applies sunrise data whether we have it or not

    emit updated(currentData_);
}

// https://api.met.no/weatherapi/locationforecast/2.0/documentation
void NMIWeatherAPI2::parseOneElement(QJsonObject &object, QHash<QDate, AbstractDailyWeatherForecast> &dayCache, QList<AbstractHourlyWeatherForecast> &hoursList)
{
    /*~~~~~~~~~~ static variable ~~~~~~~~~~~*/
    // rank weather (for what best describes the day overall)
    static const QHash<QString, int> rank = {// only need neutral icons
                                             {"weather-none-available", -1},
                                             {"weather-clear", 0},
                                             {"weather-few-clouds", 1},
                                             {"weather-clouds", 2},
                                             {"weather-fog", 3},
                                             {"weather-mist", 3},
                                             {"weather-showers-scattered", 4},
                                             {"weather-snow-scattered", 4},
                                             {"weather-showers", 5},
                                             {"weather-hail", 5},
                                             {"weather-snow", 5},
                                             {"weather-freezing-rain", 6},
                                             {"weather-freezing-storm", 6},
                                             {"weather-snow-rain", 6},
                                             {"weather-storm", 7}};

    QJsonObject data = object["data"].toObject(), instant = data["instant"].toObject()["details"].toObject();
    // ignore last forecast, which does not have enough data
    if (!data.contains("next_6_hours") && !data.contains("next_1_hours"))
        return;

    // correct date to corresponding timezone of location if possible
    QDateTime date;
    if (timeZone_.isEmpty()) {
        date = QDateTime::fromString(object.value("time").toString(), Qt::ISODate);
    } else {
        date = QDateTime::fromString(object.value("time").toString(), Qt::ISODate).toTimeZone(QTimeZone(timeZone_.toUtf8()));
    }

    AbstractHourlyWeatherForecast hourForecast;

    // set initial hour fields
    hourForecast.setDate(date); // the first time will be at the exact time of
                                // query, otherwise the beginning of each hour
    hourForecast.setTemperature(instant["air_temperature"].toDouble());
    hourForecast.setPressure(instant["air_pressure_at_sea_level"].toDouble());
    hourForecast.setWindSpeed(instant["wind_speed"].toDouble());
    hourForecast.setHumidity(instant["relative_humidity"].toDouble());
    hourForecast.setFog(instant["fog_area_fraction"].toDouble());
    hourForecast.setUvIndex(instant["ultraviolet_index_clear_sky"].toDouble());

    // wind direction
    double windDirectionDeg = instant["wind_from_direction"].toDouble();
    hourForecast.setWindDirection(getWindDirect(windDirectionDeg));

    QString symbolCode;
    // some fields contain only "next_1_hours", and others may contain only
    // "next_6_hours"
    if (data.contains("next_1_hours")) {
        QJsonObject nextOneHours = data["next_1_hours"].toObject();
        symbolCode = nextOneHours["summary"].toObject()["symbol_code"].toString("unknown");
        hourForecast.setPrecipitationAmount(nextOneHours["details"].toObject()["precipitation_amount"].toDouble());
    } else {
        QJsonObject nextSixHours = data["next_6_hours"].toObject();
        symbolCode = nextSixHours["summary"].toObject()["symbol_code"].toString("unknown");
        hourForecast.setPrecipitationAmount(nextSixHours["details"].toObject()["precipitation_amount"].toDouble());
    }

    symbolCode = symbolCode.split('_')[0]; // trim _[day/night] from end -
                                           // https://api.met.no/weatherapi/weathericon/2.0/legends
    hourForecast.setNeutralWeatherIcon(apiDescMap[symbolCode + "_neutral"].icon);
    hourForecast.setSymbolCode(symbolCode);

    // add day if not already created
    if (!dayCache.contains(date.date())) {
        dayCache[date.date()] = AbstractDailyWeatherForecast(-1e9, 1e9, 0, 0, 0, 0, "weather-none-available", "Unknown", date.date());
    }

    // update day forecast with hour information if needed
    AbstractDailyWeatherForecast &dayForecast = dayCache[date.date()];

    dayForecast.setPrecipitation(dayForecast.precipitation() + hourForecast.precipitationAmount());
    dayForecast.setUvIndex(std::max(dayForecast.uvIndex(), hourForecast.uvIndex()));
    dayForecast.setHumidity(std::max(dayForecast.humidity(), hourForecast.humidity()));
    dayForecast.setPressure(std::max(dayForecast.pressure(), hourForecast.pressure()));

    if (data.contains("next_6_hours")) {
        QJsonObject details = data["next_6_hours"].toObject()["details"].toObject();
        dayForecast.setMaxTemp(std::max(dayForecast.maxTemp(), (float)details["air_temperature_max"].toDouble()));
        dayForecast.setMinTemp(std::min(dayForecast.minTemp(), (float)details["air_temperature_min"].toDouble()));
    }

    // set description and icon if it is higher ranked
    if (rank[hourForecast.neutralWeatherIcon()] >= rank[dayForecast.weatherIcon()]) {
        dayForecast.setWeatherDescription(apiDescMap[symbolCode + "_neutral"].desc);
        dayForecast.setWeatherIcon(hourForecast.neutralWeatherIcon());
    }

    // add hour forecast to list
    hoursList.append(hourForecast);
}
