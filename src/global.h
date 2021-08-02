/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once
#include "kweathersettings.h"
#include <KLocalizedContext>
#include <KLocalizedString>
#include <QString>
#include <unordered_map>
namespace Kweather
{
static const QString WEATHER_LOCATIONS_CFG_GROUP = QStringLiteral("WeatherLocations");

static double convertTemp(double temp)
{
    if (KWeatherSettings().temperatureUnits() == "Fahrenheit") {
        return temp * 1.8 + 32;
    } else {
        return temp;
    }
};

static QString tempConverter(double temp)
{
    return QString::number(qRound(convertTemp(temp))) + "°";
}

enum class WindDirection { N, NW, W, SW, S, SE, E, NE };
enum class Backend { NMI, OWM };
struct ResolvedWeatherDesc {
    QString icon = "weather-none-available", desc = "Unknown";
    ResolvedWeatherDesc() = default;
    ResolvedWeatherDesc(QString icon, QString desc)
    {
        this->icon = icon;
        this->desc = desc;
    }
};

static const QString API_NMI = "Norway Meteorologisk Institutt";
static const QString API_OWM = "OpenWeatherMap";

}
