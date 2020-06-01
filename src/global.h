/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#ifndef ICONMAP_H
#define ICONMAP_H
#include <KLocalizedContext>
#include <KLocalizedString>
#include <QString>
#include <unordered_map>
namespace Kweather
{
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

static const QString API_NMI = "nmiweatherapi";
static const QString API_OWM = "owmweatherapi";
}
#endif // ICONMAP_H
