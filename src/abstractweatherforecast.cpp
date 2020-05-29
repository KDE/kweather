/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "abstractweatherforecast.h"
#include "abstractsunrise.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <utility>
AbstractWeatherForecast::AbstractWeatherForecast(QDateTime timeCreated)
{
    this->timeCreated_ = timeCreated;
}
AbstractWeatherForecast::AbstractWeatherForecast(QDateTime timeCreated,
                                                 QString locationId,
                                                 float latitude,
                                                 float longitude,
                                                 QList<AbstractHourlyWeatherForecast *> hourlyForecasts,
                                                 QList<AbstractDailyWeatherForecast *> dailyForecasts)
    : timeCreated_(timeCreated)
    , locationId_(std::move(locationId))
    , latitude_(latitude)
    , longitude_(longitude)
    , hourlyForecasts_(hourlyForecasts)
    , dailyForecasts_(dailyForecasts)
{
}

AbstractWeatherForecast::~AbstractWeatherForecast()
{
    for (auto hour : hourlyForecasts_)
        delete hour;
    for (auto day : dailyForecasts_)
        delete day;
}

AbstractWeatherForecast *AbstractWeatherForecast::fromJson(QJsonObject obj)
{
    auto *fc = new AbstractWeatherForecast();
    fc->setTimeCreated(QDateTime::fromString(obj["timeCreated"].toString(), Qt::ISODate));
    fc->setLocationId(obj["locationId"].toString());
    fc->setLatitude(obj["latitude"].toString().toDouble());
    fc->setLongitude(obj["longitude"].toString().toDouble());
    QList<AbstractHourlyWeatherForecast *> hourList;
    QList<AbstractDailyWeatherForecast *> dayList;
    QList<AbstractSunrise *> sunriseList;

    for (auto hour : obj["hourlyForecasts"].toArray())
        hourList.push_back(AbstractHourlyWeatherForecast::fromJson(hour.toObject()));
    for (auto day : obj["dailyForecasts"].toArray())
        dayList.push_back(AbstractDailyWeatherForecast::fromJson(day.toObject()));
    for (auto sr : obj["sunrise"].toArray())
        sunriseList.push_back(AbstractSunrise::fromJson(sr.toObject()));

    fc->setHourlyForecasts(hourList);
    fc->setDailyForecasts(dayList);
    fc->setSunrise(sunriseList);
    return fc;
}

QJsonObject AbstractWeatherForecast::toJson()
{
    QJsonObject obj;
    obj["timeCreated"] = this->timeCreated().toString(Qt::ISODate);
    obj["locationId"] = QString(this->locationId());
    obj["latitude"] = QString::number(this->latitude());
    obj["longitude"] = QString::number(this->longitude());

    QJsonArray hourArray;
    QJsonArray dayArray;
    QJsonArray sunriseArray;

    for (auto fc : this->hourlyForecasts())
        hourArray.push_back(fc->toJson());
    for (auto fc : this->dailyForecasts())
        dayArray.push_back(fc->toJson());
    for (auto fc : this->sunrise_)
        sunriseArray.push_back(fc->toJson());

    obj["hourlyForecasts"] = hourArray;
    obj["dailyForecasts"] = dayArray;
    obj["sunrise"] = sunriseArray;
    return obj;
}
