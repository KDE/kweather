/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "weatherforecastmanager.h"
#include "weatherlocation.h"
#include "weatherlocationmodel.h"
#include <KConfigCore/KConfigGroup>
#include <QDirIterator>
#include <QFile>
#include <QStandardPaths>
#include <QTimeZone>
#include <QTimer>

WeatherForecastManager::WeatherForecastManager()
    : m_model(new WeatherLocationListModel(this))
{
    update();
}

WeatherForecastManager *WeatherForecastManager::inst()
{
    static WeatherForecastManager singleton;
    return &singleton;
}
void WeatherForecastManager::update()
{
    qDebug() << "update start";
    auto &locations = m_model->locations();
    for (auto location : locations) {
        location->update();
    }
}
