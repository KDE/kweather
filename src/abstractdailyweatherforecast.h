/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef ABSTRACTDAILYWEATHERFORECAST_H
#define ABSTRACTDAILYWEATHERFORECAST_H
#include <QDate>
#include <QObject>

class AbstractDailyWeatherForecast
{

public:
    AbstractDailyWeatherForecast();
    AbstractDailyWeatherForecast(float maxTemp, float minTemp, float precipitation, float uvIndex, float humidity, float pressure, QString weatherIcon, QString weatherDescription, QDate date);

    QJsonObject toJson();
    static AbstractDailyWeatherForecast fromJson(QJsonObject obj);

    inline void setMaxTemp(float maxTemp)
    {
        maxTemp_ = maxTemp;
    }
    inline void setMinTemp(float minTemp)
    {
        minTemp_ = minTemp;
    }
    inline void setPrecipitation(float precipitation)
    {
        precipitation_ = precipitation;
    };
    inline void setUvIndex(float uvIndex)
    {
        uvIndex_ = uvIndex;
    }
    inline void setHumidity(float humidity)
    {
        humidity_ = humidity;
    }
    inline void setPressure(float pressure)
    {
        pressure_ = pressure;
    }
    inline void setWeatherIcon(QString icon)
    {
        weatherIcon_ = icon;
    };
    inline void setWeatherDescription(QString description)
    {
        weatherDescription_ = description;
    }
    inline void setDate(QDate date)
    {
        date_ = date;
    }
    inline float maxTemp() const
    {
        return maxTemp_;
    }
    inline float minTemp() const
    {
        return minTemp_;
    }
    inline float precipitation() const
    {
        return precipitation_;
    };
    inline float uvIndex() const
    {
        return uvIndex_;
    }
    inline float humidity() const
    {
        return humidity_;
    }
    inline float pressure() const
    {
        return pressure_;
    }
    inline QString &weatherIcon()
    {
        return weatherIcon_;
    };
    inline QString &weatherDescription()
    {
        return weatherDescription_;
    }
    inline QDate date()
    {
        return date_;
    }

private:
    float maxTemp_{};
    float minTemp_{};
    float precipitation_{}; // mm
    float uvIndex_{}; // 0-1
    float humidity_{}; // %
    float pressure_{}; // hPa
    QString weatherIcon_;
    QString weatherDescription_;
    QDate date_;
};

#endif // ABSTRACTDAILYWEATHERFORECAST_H
