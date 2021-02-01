/*
 * Copyright 2020 2021 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "weatherhour.h"
#include "weatherhourmodel.h"
#include "global.h"

WeatherHour::WeatherHour(WeatherHourListModel *parent)
    : QObject(parent)
{
    if(parent)
        connect(parent, &WeatherHourListModel::weatherRefresh, this, &WeatherHour::refreshHoursFromForecasts);
}

WeatherHour::WeatherHour(SharedForecastPtr ptr, int day, int hour, WeatherHourListModel *parent)
    : QObject(parent)
    , m_forecast(ptr)
    , m_day(day)
    , m_hour(hour)
{
}
void WeatherHour::refreshHoursFromForecasts(SharedForecastPtr ptr)
{
    m_forecast = ptr;
    Q_EMIT propertyChanged();
}
QString WeatherHour::windSpeed() const
{
    if (KWeatherSettings().speedUnits() == "kph")
        return QString::number(hour().windSpeed(), 'g', 1) + "km/h";
    else
        return QString::number(hour().windSpeed() * 0.62, 'g', 1) + "mph";
}
const KWeatherCore::HourlyWeatherForecast &WeatherHour::hour() const
{
    return m_forecast->dailyWeatherForecast().at(m_day).hourlyWeatherForecast().at(m_hour);
}
