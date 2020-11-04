/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "weatherday.h"
#include "global.h"
WeatherDay::WeatherDay()
{
}

WeatherDay::WeatherDay(const KWeatherCore::DailyWeatherForecast &dailyForecast)
{
    this->m_maxTemp = dailyForecast.maxTemp();
    this->m_minTemp = dailyForecast.minTemp();
    this->m_weatherIcon = dailyForecast.weatherIcon();
    this->m_weatherDescription = dailyForecast.weatherDescription();
    this->m_date = dailyForecast.date();
    this->m_precipitation = dailyForecast.precipitation();
    this->m_uvIndex = dailyForecast.uvIndex();
    this->m_humidity = dailyForecast.humidity();
    this->m_pressure = dailyForecast.pressure();

    this->m_sunrise = dailyForecast.sunrise().sunRise().toString(QStringLiteral("hh:mm ap"));
    this->m_sunset = dailyForecast.sunrise().sunSet().toString(QStringLiteral("hh:mm ap"));
    if (dailyForecast.sunrise().moonPhase() <= 5) {
        this->m_moonPhase = "New Moon";
    } else if (dailyForecast.sunrise().moonPhase() <= 25) {
        this->m_moonPhase = "Waxing Crescent";
    } else if (dailyForecast.sunrise().moonPhase() <= 45) {
        this->m_moonPhase = "Waxing Gibbous";
    } else if (dailyForecast.sunrise().moonPhase() <= 55) {
        this->m_moonPhase = "Full Moon";
    } else if (dailyForecast.sunrise().moonPhase() <= 75) {
        this->m_moonPhase = "Waning Gibbous";
    } else if (dailyForecast.sunrise().moonPhase() <= 95) {
        this->m_moonPhase = "Waning Crescent";
    } else {
        this->m_moonPhase = "New Moon";
    }
}

const QString &WeatherDay::weatherDescription() const
{
    return m_weatherDescription;
}
const QString &WeatherDay::weatherIcon() const
{
    return m_weatherIcon;
}
const QDate &WeatherDay::date() const
{
    return m_date;
}
QString WeatherDay::maxTemp() const
{
    return Kweather::tempConverter(m_maxTemp);
}
QString WeatherDay::minTemp() const
{
    return Kweather::tempConverter(m_minTemp);
}
float WeatherDay::precipitation() const
{
    return m_precipitation;
}
float WeatherDay::uvIndex() const
{
    return m_uvIndex;
}
float WeatherDay::humidity() const
{
    return m_humidity;
}
float WeatherDay::pressure() const
{
    return m_pressure;
}
const QString &WeatherDay::sunrise() const
{
    return m_sunrise;
}
const QString &WeatherDay::sunset() const
{
    return m_sunset;
}
const QString &WeatherDay::moonPhase() const
{
    return m_moonPhase;
}
