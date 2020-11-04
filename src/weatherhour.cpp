/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "weatherhour.h"
#include "global.h"

WeatherHour::WeatherHour()
{
    this->m_weatherDescription = "Unknown";
    this->m_weatherIcon = "weather-none-available";
    this->m_date = QDateTime::currentDateTime();
    this->m_windDirection = "N";
}

WeatherHour::WeatherHour(const KWeatherCore::HourlyWeatherForecast &forecast)
{
    switch (forecast.windDirection()) {
    case KWeatherCore::WindDirection::N:
        this->m_windDirection = "N";
        break;
    case KWeatherCore::WindDirection::NE:
        this->m_windDirection = "NE";
        break;
    case KWeatherCore::WindDirection::E:
        this->m_windDirection = "E";
        break;
    case KWeatherCore::WindDirection::SE:
        this->m_windDirection = "SE";
        break;
    case KWeatherCore::WindDirection::S:
        this->m_windDirection = "S";
        break;
    case KWeatherCore::WindDirection::SW:
        this->m_windDirection = "SW";
        break;
    case KWeatherCore::WindDirection::W:
        this->m_windDirection = "W";
        break;
    case KWeatherCore::WindDirection::NW:
        this->m_windDirection = "NW";
        break;
    }
    this->m_weatherDescription = forecast.weatherDescription();
    this->m_weatherIcon = forecast.weatherIcon();
    this->m_precipitation = forecast.precipitationAmount();
    this->m_fog = forecast.fog();
    this->m_windSpeed = forecast.windSpeed();
    this->m_temperature = forecast.temperature();
    this->m_humidity = forecast.humidity();
    this->m_pressure = forecast.pressure();
    this->m_date = QDateTime(forecast.date().date(), QTime(forecast.date().time().hour(), 0));
}
const QString &WeatherHour::windDirection() const
{
    return m_windDirection;
}
const QString &WeatherHour::weatherDescription() const
{
    return m_weatherDescription;
}
const QString &WeatherHour::weatherIcon() const
{
    return m_weatherIcon;
}
float WeatherHour::precipitation() const
{
    return m_precipitation;
}
float WeatherHour::fog() const
{
    return m_fog;
}
QString WeatherHour::windSpeed() const
{
    if (KWeatherSettings().speedUnits() == "kph")
        return QString::number(m_windSpeed, 'g', 1) + "km/h";
    else
        return QString::number(m_windSpeed * 0.62, 'g', 1) + "mph";
}
QString WeatherHour::temperatureRounded() const
{
    return Kweather::tempConverter(m_temperature);
}
float WeatherHour::humidity() const
{
    return m_humidity;
}
float WeatherHour::pressure() const
{
    return m_pressure;
}
const QDateTime &WeatherHour::date() const
{
    return m_date;
}

void WeatherHour::setWindDirection(const QString &windDirection)
{
    m_windDirection = windDirection;
}
void WeatherHour::setWeatherDescription(const QString &weatherDescription)
{
    m_weatherDescription = weatherDescription;
}
void WeatherHour::setWeatherIcon(const QString &weatherIcon)
{
    m_weatherIcon = weatherIcon;
}
void WeatherHour::setPrecipitation(float precipitation)
{
    m_precipitation = precipitation;
}
void WeatherHour::setFog(float fog)
{
    m_fog = fog;
}
void WeatherHour::setWindSpeed(float windSpeed)
{
    m_windSpeed = windSpeed;
}
void WeatherHour::setTemperature(float temperature)
{
    m_temperature = temperature;
}
void WeatherHour::setHumidity(float humidity)
{
    m_humidity = humidity;
}
void WeatherHour::setPressure(float pressure)
{
    m_pressure = pressure;
}
void WeatherHour::setDate(const QDateTime &date)
{
    m_date = date;
}
