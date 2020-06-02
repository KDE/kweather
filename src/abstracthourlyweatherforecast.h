/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef KWEATHER_ABSTRACTHOURLYWEATHERFORECAST_H
#define KWEATHER_ABSTRACTHOURLYWEATHERFORECAST_H

#include "global.h"
#include <QDateTime>
#include <QDebug>
#include <QObject>
#include <QSettings>

class AbstractHourlyWeatherForecast
{

public:
    AbstractHourlyWeatherForecast();
    AbstractHourlyWeatherForecast(QDateTime date,
                                  QString weatherDescription,
                                  QString weatherIcon,
                                  QString neutralWeatherIcon,
                                  float temperature,
                                  float pressure,
                                  Kweather::WindDirection windDirection,
                                  float windSpeed,
                                  float humidity,
                                  float fog,
                                  float uvIndex,
                                  float precipitationAmount);

    QJsonObject toJson();
    static AbstractHourlyWeatherForecast fromJson(QJsonObject obj);

    const QDateTime &date() const
    {
        return date_;
    }
    void setDate(const QDateTime &date)
    {
        date_ = date;
    }
    const QString &weatherDescription() const
    {
        return weatherDescription_;
    }
    void setWeatherDescription(const QString &weatherDescription)
    {
        weatherDescription_ = weatherDescription;
    }
    const QString &weatherIcon() const
    {
        return weatherIcon_;
    }
    void setWeatherIcon(const QString &weatherIcon)
    {
        weatherIcon_ = weatherIcon;
    }
    const QString &neutralWeatherIcon() const
    {
        return neutralWeatherIcon_;
    }
    void setNeutralWeatherIcon(const QString &neutralWeatherIcon)
    {
        neutralWeatherIcon_ = neutralWeatherIcon;
    }
    const QString &symbolCode() const
    {
        return symbolCode_;
    }
    void setSymbolCode(const QString &symbolCode)
    {
        symbolCode_ = symbolCode;
    }
    QString temperatureFormatted() const
    {
        QSettings settings;
        if (settings.value("Global/temperatureUnits", "Celsius").toString() == "Fahrenheit") {
            return QString::number(qRound(temperature_ * 1.8 + 32)) + "°";
        } else {
            return QString::number(qRound(temperature_)) + "°";
        }
    }
    float temperature() const
    {
        return temperature_;
    }
    void setTemperature(float temperature)
    {
        temperature_ = temperature;
    }
    float pressure() const
    {
        return pressure_;
    }
    void setPressure(float pressure)
    {
        pressure_ = pressure;
    }
    Kweather::WindDirection windDirection() const
    {
        return windDirection_;
    }
    void setWindDirection(Kweather::WindDirection windDirection)
    {
        windDirection_ = windDirection;
    }
    float windSpeed() const
    {
        return windSpeed_;
    }
    void setWindSpeed(float windSpeed)
    {
        windSpeed_ = windSpeed;
    }
    float humidity() const
    {
        return humidity_;
    }
    void setHumidity(float humidity)
    {
        humidity_ = humidity;
    }
    float fog() const
    {
        return fog_;
    }
    void setFog(float fog)
    {
        fog_ = fog;
    }
    float uvIndex() const
    {
        return uvIndex_;
    }
    void setUvIndex(float uvIndex)
    {
        uvIndex_ = uvIndex;
    }
    float precipitationAmount() const
    {
        return precipitationAmount_;
    }
    void setPrecipitationAmount(float precipitationAmount)
    {
        precipitationAmount_ = precipitationAmount;
    }

private:
    QDateTime date_;
    QString weatherDescription_;
    QString weatherIcon_;
    QString neutralWeatherIcon_; // weather icon without time of day
    QString symbolCode_;
    float temperature_ {}; // celsius
    float pressure_ {};    // hPa
    Kweather::WindDirection windDirection_;
    float windSpeed_ {};           // m/s
    float humidity_ {};            // %
    float fog_ {};                 // %
    float uvIndex_ {};             // 0-1
    float precipitationAmount_ {}; // mm
};

#endif // KWEATHER_ABSTRACTHOURLYWEATHERFORECAST_H
