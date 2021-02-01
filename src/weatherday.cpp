/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "weatherday.h"
#include "weatherdaymodel.h"
#include "global.h"
WeatherDay::WeatherDay(WeatherDayListModel *parent)
    : QObject(parent)
{
    if(parent)
        connect(parent, &WeatherDayListModel::weatherRefresh, this, &WeatherDay::refreshDaysFromForecasts);
}

WeatherDay::WeatherDay(SharedForecastPtr ptr, int day, WeatherDayListModel *parent)
    :QObject(parent)
    ,m_day(day)
    ,m_forecast(ptr)
{
    if(parent)
        connect(parent, &WeatherDayListModel::weatherRefresh, this, &WeatherDay::refreshDaysFromForecasts);

    determineSunrise();
}
void WeatherDay::determineSunrise()
{
    m_sunrise = day().sunrise().sunRise().toString(QStringLiteral("hh:mm ap"));
    m_sunset = day().sunrise().sunSet().toString(QStringLiteral("hh:mm ap"));
    if (day().sunrise().moonPhase() <= 5) {
        m_moonPhase = "New Moon";
    } else if (day().sunrise().moonPhase() <= 25) {
        m_moonPhase = "Waxing Crescent";
    } else if (day().sunrise().moonPhase() <= 45) {
        m_moonPhase = "Waxing Gibbous";
    } else if (day().sunrise().moonPhase() <= 55) {
        m_moonPhase = "Full Moon";
    } else if (day().sunrise().moonPhase() <= 75) {
        m_moonPhase = "Waning Gibbous";
    } else if (day().sunrise().moonPhase() <= 95) {
        m_moonPhase = "Waning Crescent";
    } else {
        m_moonPhase = "New Moon";
    }
}
void WeatherDay::refreshDaysFromForecasts(SharedForecastPtr ptr)
{
    m_forecast = ptr;
    determineSunrise();
    Q_EMIT propertyChanged();
}
