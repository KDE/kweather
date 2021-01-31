/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include "kweathersettings.h"
#include <KWeatherCore/HourlyWeatherForecast>
#include <QObject>
class WeatherHour : public KWeatherCore::HourlyWeatherForecast
{
    Q_GADGET
    Q_PROPERTY(QString windDirection READ windDirection)
    Q_PROPERTY(QString windSpeed READ windSpeed)
    Q_PROPERTY(QString temperatureRounded READ temperatureRounded)
public:
    WeatherHour();
    explicit WeatherHour(const KWeatherCore::HourlyWeatherForecast &forecast);

    const QString &windDirection() const;
    QString windSpeed() const;
    QString temperatureRounded() const;
private:
    QString m_windDirection;
};
Q_DECLARE_METATYPE(WeatherHour)
