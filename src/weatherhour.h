/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include "global.h"
#include "kweathersettings.h"
#include <KWeatherCore/WeatherForecast>
#include <QObject>

class WeatherHourListModel;
class WeatherHour : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString windDirection READ windDirection NOTIFY propertyChanged)
    Q_PROPERTY(QString weatherDescription READ weatherDescription NOTIFY propertyChanged)
    Q_PROPERTY(QString weatherIcon READ weatherIcon NOTIFY propertyChanged)
    Q_PROPERTY(qreal precipitation READ precipitation NOTIFY propertyChanged)
    Q_PROPERTY(qreal fog READ fog NOTIFY propertyChanged)
    Q_PROPERTY(QString windSpeed READ windSpeed NOTIFY propertyChanged)
    Q_PROPERTY(QString temperature READ temperature NOTIFY propertyChanged)
    Q_PROPERTY(QString temperatureRounded READ temperatureRounded NOTIFY propertyChanged)
    Q_PROPERTY(qreal humidity READ humidity NOTIFY propertyChanged)
    Q_PROPERTY(qreal pressure READ pressure NOTIFY propertyChanged)
    Q_PROPERTY(QDateTime date READ date NOTIFY propertyChanged)

public:
    WeatherHour(WeatherHourListModel *parent = nullptr);
    explicit WeatherHour(KWeatherCore::WeatherForecast ptr, int day, int hour, WeatherHourListModel *parent = nullptr);

    QString windDirection() const
    {
        return hour().windDirectionStr();
    }
    const QString &weatherDescription() const
    {
        return hour().weatherDescription();
    }
    const QString &weatherIcon() const
    {
        return hour().weatherIcon();
    }
    qreal precipitation() const
    {
        return hour().precipitationAmount();
    }
    qreal fog() const
    {
        return hour().fog();
    }
    QString windSpeed() const;
    QString temperature() const
    {
        return Kweather::tempConverter(hour().temperature());
    }
    QString temperatureRounded() const
    {
        return Kweather::tempConverter(hour().temperature());
    }
    qreal humidity() const
    {
        return hour().humidity();
    }
    qreal pressure() const
    {
        return hour().pressure();
    }
    const QDateTime &date() const
    {
        return hour().date();
    }
public Q_SLOTS:
    void refreshHoursFromForecasts(KWeatherCore::WeatherForecast ptr);
Q_SIGNALS:
    void propertyChanged();

private:
    const KWeatherCore::HourlyWeatherForecast &hour() const;

    KWeatherCore::WeatherForecast m_forecast;
    int m_day = 0;
    int m_hour = 0;
};
