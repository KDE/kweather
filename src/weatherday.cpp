/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "weatherday.h"
#include "global.h"
WeatherDay::WeatherDay()
    :KWeatherCore::DailyWeatherForecast(KWeatherCore::DailyWeatherForecast(QDate::currentDate()))
{
}

WeatherDay::WeatherDay(const KWeatherCore::DailyWeatherForecast &dailyForecast)
    :KWeatherCore::DailyWeatherForecast(dailyForecast)
{
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
