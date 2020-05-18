/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef ABSTRACTWEATHERFORECAST_H
#define ABSTRACTWEATHERFORECAST_H

#include "abstractdailyweatherforecast.h"
#include "abstracthourlyweatherforecast.h"
#include <QDateTime>
#include <QObject>
#include <utility>

class AbstractSunrise;
class AbstractWeatherForecast : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDateTime timeCreated READ timeCreated WRITE setTimeCreated NOTIFY propertyChanged)
    Q_PROPERTY(float latitude READ latitude WRITE setLatitude NOTIFY propertyChanged)
    Q_PROPERTY(float longitude READ longitude WRITE setLongitude NOTIFY propertyChanged)

public:
    AbstractWeatherForecast(QDateTime timeCreated_ = QDateTime::currentDateTime());
    ~AbstractWeatherForecast();
    AbstractWeatherForecast(QDateTime timeCreated_, QString locationId, float latitude, float longitude, QList<AbstractHourlyWeatherForecast *> hourlyForecasts, QList<AbstractDailyWeatherForecast *> dailyForecasts);

    QJsonObject toJson();
    static AbstractWeatherForecast *fromJson(QJsonObject obj);

    inline const QString &locationId()
    {
        return locationId_;
    }
    inline QDateTime timeCreated()
    {
        return timeCreated_;
    }
    inline float latitude() const
    {
        return latitude_;
    }
    inline float longitude() const
    {
        return longitude_;
    }
    const QList<AbstractHourlyWeatherForecast *> &hourlyForecasts() const
    {
        return hourlyForecasts_;
    }
    const QList<AbstractDailyWeatherForecast *> &dailyForecasts() const
    {
        return dailyForecasts_;
    }
    const QList<AbstractSunrise *> &sunrise()
    {
        return sunrise_;
    };
    inline void setLocationId(QString n)
    {
        locationId_ = std::move(n);
    }
    inline void setTimeCreated(QDateTime timeCreated)
    {
        timeCreated_ = std::move(timeCreated);
    }
    inline void setLatitude(float l)
    {
        latitude_ = l;
    }
    inline void setLongitude(float l)
    {
        longitude_ = l;
    }
    void setHourlyForecasts(const QList<AbstractHourlyWeatherForecast *> &hourlyForecasts)
    {
        hourlyForecasts_ = hourlyForecasts;
    }
    void setDailyForecasts(const QList<AbstractDailyWeatherForecast *> &dailyForecasts)
    {
        dailyForecasts_ = dailyForecasts;
    }
    void setSunrise(const QList<AbstractSunrise *> &sunrise)
    {
        sunrise_ = sunrise;
    };
    void sortDailyForecast();
signals:
    void propertyChanged();

private:
    QString locationId_;
    QDateTime timeCreated_;
    float latitude_;
    float longitude_;
    QList<AbstractHourlyWeatherForecast *> hourlyForecasts_;
    QList<AbstractDailyWeatherForecast *> dailyForecasts_;
    QList<AbstractSunrise *> sunrise_; // may be empty, as this is fetched from a separate api; do not display on ui if it is empty
};

#endif // ABSTRACTWEATHERFORECAST_H
