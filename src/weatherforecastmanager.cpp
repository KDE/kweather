/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "weatherforecastmanager.h"

#include <QDirIterator>
#include <QFile>
#include <QStandardPaths>
#include <QTimeZone>
#include <QTimer>

#include <KConfigCore/KConfigGroup>

#include "weatherlocation.h"
#include "weatherlocationmodel.h"

WeatherForecastManager::WeatherForecastManager()
    : m_model(new WeatherLocationListModel(this))
{
}

WeatherForecastManager *WeatherForecastManager::inst()
{
    static WeatherForecastManager singleton;
    return &singleton;
}
