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
#include <QExplicitlySharedDataPointer>
#include <QFile>
#include <QStandardPaths>
#include <QTimeZone>
#include <QTimer>

WeatherForecastManager::WeatherForecastManager()
    : m_model(new WeatherLocationListModel(this))
{
    // create cache location if it does not exist, and load cache
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/cache");
    if (!dir.exists())
        dir.mkpath(".");
    readFromCache();
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

void WeatherForecastManager::readFromCache()
{
    QHash<WeatherLocation *, QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast>> map;

    QFile reader;
    QDirIterator iterator(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/cache", QDirIterator::Subdirectories); // list directory entries

    while (iterator.hasNext()) {
        reader.setFileName(iterator.next());
        QFileInfo fileName(reader); // strip absolute path
        bool isFound = false; // indicate should we load this cache

        // loop over existing locations and add cached weather forecast data if location found
        for (auto location : m_model->locations()) {
            if (fileName.fileName() == location->locationId()) {
                isFound = true;
                reader.open(QIODevice::ReadOnly);
                auto fc = KWeatherCore::WeatherForecast::fromJson(QJsonDocument::fromJson(reader.readAll()).object());

                map[location] = fc;
                break;
            }
        }

        if (!isFound) { // delete no longer needed cache
            reader.remove();
        }
        reader.close();
    }

    // add to loaded locations from cache
    for (auto location : m_model->locations()) {
        if (map.find(location) != map.end()) { // is in cache
            location->initData(map[location]);
        }
    }

    update();
}
