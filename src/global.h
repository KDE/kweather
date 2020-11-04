/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
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
QString tempConverter(double temp)
{
    if (KWeatherSettings().temperatureUnits() == "Fahrenheit") {
        return QString::number(qRound(temp * 1.8 + 32)) + "°";
    } else {
        return QString::number(qRound(temp)) + "°";
    }
};
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
