/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include "kweathersettings.h"
#include <KWeatherCore/HourlyForecast>
#include <QObject>
class WeatherHour : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString windDirection READ windDirection NOTIFY propertyChanged)
    Q_PROPERTY(QString weatherDescription READ weatherDescription NOTIFY propertyChanged)
    Q_PROPERTY(QString weatherIcon READ weatherIcon NOTIFY propertyChanged)
    Q_PROPERTY(float precipitation READ precipitation NOTIFY propertyChanged)
    Q_PROPERTY(float fog READ fog NOTIFY propertyChanged)
    Q_PROPERTY(QString windSpeed READ windSpeed NOTIFY propertyChanged)
    Q_PROPERTY(QString temperatureRounded READ temperatureRounded NOTIFY propertyChanged)
    Q_PROPERTY(float humidity READ humidity NOTIFY propertyChanged)
    Q_PROPERTY(float pressure READ pressure NOTIFY propertyChanged)
    Q_PROPERTY(QDateTime date READ date NOTIFY propertyChanged)

public:
    explicit WeatherHour();
    explicit WeatherHour(const KWeatherCore::HourlyWeatherForecast &forecast);

    const QString &windDirection() const;
    const QString &weatherDescription() const;
    const QString &weatherIcon() const;
    float precipitation() const;
    float fog() const;
    QString windSpeed() const;
    QString temperatureRounded() const;
    float humidity() const;
    float pressure() const;
    const QDateTime &date() const;

    void setWindDirection(const QString &windDirection);
    void setWeatherDescription(const QString &weatherDescription);
    void setWeatherIcon(const QString &weatherIcon);
    void setPrecipitation(float precipitation);
    void setFog(float fog);
    void setWindSpeed(float windSpeed);
    void setTemperature(float temperature);
    void setHumidity(float humidity);
    void setPressure(float pressure);
    void setDate(const QDateTime &date);
Q_SIGNALS:
    void propertyChanged();

private:
    QString m_windDirection;
    QString m_weatherDescription;
    QString m_weatherIcon;
    float m_precipitation;
    float m_fog;
    float m_windSpeed;
    float m_temperature;
    float m_humidity;
    float m_pressure;

    QDateTime m_date;
};
