/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once

#include <QLocale>
#include <QString>

#include <KLocalizedContext>
#include <KLocalizedString>

#include <unordered_map>

#include "kweathersettings.h"

namespace KWeather
{
static const QString WEATHER_LOCATIONS_CFG_GROUP = QStringLiteral("WeatherLocations");

static bool isCelsius(const QString &unit)
{
    if (unit == QLatin1String("Use System Default")) {
        return (QLocale().measurementSystem() == QLocale::MetricSystem);
    }
    return unit == QLatin1String("Celsius");
};

static double convertTemp(double temp, const QString &unit)
{
    if (KWeather::isCelsius(unit)) {
        return temp;
    } else {
        return temp * 1.8 + 32;
    }
};

enum class WindDirection { N, NW, W, SW, S, SE, E, NE };
enum class Backend { NMI, OWM };
}
