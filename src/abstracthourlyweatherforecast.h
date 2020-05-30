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

class AbstractHourlyWeatherForecast : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString weatherDescription READ weatherDescription WRITE setWeatherDescription NOTIFY propertyChanged)
    Q_PROPERTY(QString weatherIcon READ weatherIcon WRITE setWeatherIcon NOTIFY propertyChanged)
    Q_PROPERTY(QString neutralWeatherIcon READ neutralWeatherIcon WRITE setNeutralWeatherIcon NOTIFY propertyChanged)
    Q_PROPERTY(QString temperature READ temperatureFormatted NOTIFY propertyChanged)
    Q_PROPERTY(float pressure READ pressure WRITE setPressure NOTIFY propertyChanged)
    Q_PROPERTY(QString windDirection READ windDirectionString NOTIFY propertyChanged)
    Q_PROPERTY(QString windSpeed READ windSpeedDisplay NOTIFY propertyChanged)
    Q_PROPERTY(float humidity READ humidity WRITE setHumidity NOTIFY propertyChanged)
    Q_PROPERTY(float fog READ fog WRITE setFog NOTIFY propertyChanged)
    Q_PROPERTY(float uvIndex READ uvIndex WRITE setUvIndex NOTIFY propertyChanged)
    Q_PROPERTY(float precipitationAmount READ precipitationAmount WRITE setPrecipitationAmount NOTIFY propertyChanged)

public:
    AbstractHourlyWeatherForecast() = default;
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
    static AbstractHourlyWeatherForecast *fromJson(QJsonObject obj);

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
    void setWeatherIcon(bool isDay)
    {
        using namespace Kweather;
        if (isDay) {
            setWeatherDescription(apiDescMap.find(symbolCode_ + "_day")->second.desc);
            weatherIcon_ = apiDescMap.find(symbolCode_ + "_day")->second.icon;
        } else {
            setWeatherDescription(apiDescMap.find(symbolCode_ + "_night")->second.desc);
            weatherIcon_ = apiDescMap.find(symbolCode_ + "_night")->second.icon;
        }
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
    QString windDirectionString() const
    {
        switch (windDirection()) {
        case Kweather::WindDirection::N:
            return "N";
        case Kweather::WindDirection::NE:
            return "NE";
        case Kweather::WindDirection::E:
            return "E";
        case Kweather::WindDirection::SE:
            return "SE";
        case Kweather::WindDirection::S:
            return "S";
        case Kweather::WindDirection::SW:
            return "SW";
        case Kweather::WindDirection::W:
            return "W";
        case Kweather::WindDirection::NW:
            return "NW";
        }
        return "";
    }
    void setWindDirection(Kweather::WindDirection windDirection)
    {
        windDirection_ = windDirection;
    }
    QString windSpeedDisplay() const
    {
        QSettings settings;
        if (settings.value("Global/speedUnits", "Kph").toString() == "kph")
            return QString::number(windSpeed_, 'g', 1) + "km/h";
        else
            return QString::number(windSpeed_ * 0.62, 'g', 1) + "mph";
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

signals:
    void propertyChanged();

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
