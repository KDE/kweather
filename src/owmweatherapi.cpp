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
        hourly = new AbstractHourlyWeatherForecast();
        hourly->setDate(QDateTime::fromSecsSinceEpoch(fc.toObject()["dt"].toInt()).toTimeZone(QTimeZone(offset)));
        hourly->setFog(-1);
        hourly->setUvIndex(-1);
        hourly->setHumidity(fc.toObject()["main"].toObject()["humidity"].toInt());
        hourly->setPressure(fc.toObject()["main"].toObject()["pressure"].toInt());
        hourly->setWindSpeed(fc.toObject()["wind"].toObject()["speed"].toDouble());
        hourly->setTemperature(fc.toObject()["main"].toObject()["temp"].toDouble());
        hourly->setWeatherIcon(map[fc.toObject()["weather"].toArray().at(0)["icon"].toString()]);
        hourly->setWindDirection(getWindDirection(fc.toObject()["wind"].toObject()["deg"].toInt()));
        hourly->setWeatherDescription(fc.toObject()["weather"].toArray().at(0)["description"].toString());
        hourly->setPrecipitationAmount(fc.toObject()["rain"].toObject()["3h"].toDouble() + fc.toObject()["snow"].toObject()["3h"].toDouble());
        hourlyList.push_back(hourly);
    }
}

AbstractHourlyWeatherForecast::WindDirection OWMWeatherAPI::getWindDirection(int windDirectionDeg)
{
    if (windDirectionDeg < 22.5 || windDirectionDeg >= 337.5) {
        return AbstractHourlyWeatherForecast::S; // from N
    } else if (windDirectionDeg > 22.5 || windDirectionDeg <= 67.5) {
        return AbstractHourlyWeatherForecast::SW; // from NE
    } else if (windDirectionDeg > 67.5 || windDirectionDeg <= 112.5) {
        return AbstractHourlyWeatherForecast::W; // from E
    } else if (windDirectionDeg > 112.5 || windDirectionDeg <= 157.5) {
        return AbstractHourlyWeatherForecast::NW; // from SE
    } else if (windDirectionDeg > 157.5 || windDirectionDeg <= 202.5) {
        return AbstractHourlyWeatherForecast::N; // from S
    } else if (windDirectionDeg > 202.5 || windDirectionDeg <= 247.5) {
        return AbstractHourlyWeatherForecast::NE; // from SW
    } else if (windDirectionDeg > 247.5 || windDirectionDeg <= 292.5) {
        return AbstractHourlyWeatherForecast::E; // from W
    } else if (windDirectionDeg > 292.5 || windDirectionDeg <= 337.5) {
        return AbstractHourlyWeatherForecast::SE; // from NW
    }
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
