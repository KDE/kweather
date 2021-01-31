/*
 * Copyright 2020 2021 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "weatherhour.h"
#include "global.h"

WeatherHour::WeatherHour()
    :KWeatherCore::HourlyWeatherForecast(KWeatherCore::HourlyWeatherForecast(QDateTime::currentDateTime()))
{
    this->m_windDirection = QStringLiteral("N");
}

WeatherHour::WeatherHour(const KWeatherCore::HourlyWeatherForecast &forecast)
    :KWeatherCore::HourlyWeatherForecast(forecast)
{
    switch (forecast.windDirection()) {
    case KWeatherCore::WindDirection::N:
        this->m_windDirection = QStringLiteral("N");
        break;
    case KWeatherCore::WindDirection::NE:
        this->m_windDirection = QStringLiteral("NE");
        break;
    case KWeatherCore::WindDirection::E:
        this->m_windDirection = QStringLiteral("E");
        break;
    case KWeatherCore::WindDirection::SE:
        this->m_windDirection = QStringLiteral("SE");
        break;
    case KWeatherCore::WindDirection::S:
        this->m_windDirection = QStringLiteral("S");
        break;
    case KWeatherCore::WindDirection::SW:
        this->m_windDirection = QStringLiteral("SW");
        break;
    case KWeatherCore::WindDirection::W:
        this->m_windDirection = QStringLiteral("W");
        break;
    case KWeatherCore::WindDirection::NW:
        this->m_windDirection = QStringLiteral("NW");
        break;
    }
}
const QString &WeatherHour::windDirection() const
{
    return m_windDirection;
}
QString WeatherHour::windSpeed() const
{
    if (KWeatherSettings().speedUnits() == "kph")
        return QString::number(KWeatherCore::HourlyWeatherForecast::windSpeed(), 'g', 1) + "km/h";
    else
        return QString::number(KWeatherCore::HourlyWeatherForecast::windSpeed() * 0.62, 'g', 1) + "mph";
}
QString WeatherHour::temperatureRounded() const
{
    return Kweather::tempConverter(KWeatherCore::HourlyWeatherForecast::temperature());
}
