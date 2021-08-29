/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once

#include <QString>

#include <KLocalizedContext>
#include <KLocalizedString>

#include <unordered_map>

#include "kweathersettings.h"

namespace Kweather
{
static const QString WEATHER_LOCATIONS_CFG_GROUP = QStringLiteral("WeatherLocations");

static double convertTemp(double temp, const QString &unit)
{
    if (unit == QLatin1String("Fahrenheit")) {
        return temp * 1.8 + 32;
    } else {
        return temp;
    }
};

enum class WindDirection { N, NW, W, SW, S, SE, E, NE };
enum class Backend { NMI, OWM };
}
