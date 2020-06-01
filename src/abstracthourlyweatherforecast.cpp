/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "abstracthourlyweatherforecast.h"

#include <QJsonObject>
#include <utility>

AbstractHourlyWeatherForecast::AbstractHourlyWeatherForecast()
{
    date_ = QDateTime::currentDateTime();
    weatherDescription_ = "Unknown";
    weatherIcon_ = "weather-none-available";
    neutralWeatherIcon_ = "weather-none-available";
}

AbstractHourlyWeatherForecast::AbstractHourlyWeatherForecast(QDateTime date,
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
                                                             float precipitationAmount)
    : date_(std::move(date))
    , weatherDescription_(std::move(weatherDescription))
    , weatherIcon_(weatherIcon)
    , neutralWeatherIcon_(neutralWeatherIcon)
    , temperature_(temperature)
    , pressure_(pressure)
    , windDirection_(windDirection)
    , windSpeed_(windSpeed)
    , humidity_(humidity)
    , fog_(fog)
    , uvIndex_(uvIndex)
    , precipitationAmount_(precipitationAmount)
{
}

AbstractHourlyWeatherForecast AbstractHourlyWeatherForecast::fromJson(QJsonObject obj)
{
    AbstractHourlyWeatherForecast fc;
    fc.setDate(QDateTime::fromString(obj["date"].toString(), Qt::ISODate));
    fc.setWeatherDescription(obj["weatherDescription"].toString());
    fc.setWeatherIcon(obj["weatherIcon"].toString());
    fc.setNeutralWeatherIcon(obj["neutralWeatherIcon"].toString());
    fc.setTemperature(obj["temperature"].toDouble());
    fc.setPressure(obj["pressure"].toDouble());
    fc.setWindDirection(static_cast<Kweather::WindDirection>(obj["windDirection"].toInt()));
    fc.setWindSpeed(obj["windSpeed"].toDouble());
    fc.setHumidity(obj["humidity"].toDouble());
    fc.setFog(obj["fog"].toDouble());
    fc.setUvIndex(obj["uvIndex"].toDouble());
    fc.setPrecipitationAmount(obj["precipitationAmount"].toDouble());
    return fc;
}

QJsonObject AbstractHourlyWeatherForecast::toJson()
{
    QJsonObject obj;
    obj[QLatin1String("date")] = date().toString(Qt::ISODate);
    obj[QLatin1String("weatherDescription")] = weatherDescription();
    obj[QLatin1String("weatherIcon")] = weatherIcon();
    obj[QLatin1String("neutralWeatherIcon")] = neutralWeatherIcon();
    obj[QLatin1String("temperature")] = temperature();
    obj[QLatin1String("pressure")] = pressure();
    obj[QLatin1String("windDirection")] = static_cast<int>(windDirection());
    obj[QLatin1String("windSpeed")] = windSpeed();
    obj[QLatin1String("humidity")] = humidity();
    obj[QLatin1String("fog")] = fog();
    obj[QLatin1String("uvIndex")] = uvIndex();
    obj[QLatin1String("precipitationAmount")] = precipitationAmount();
    return obj;
}
