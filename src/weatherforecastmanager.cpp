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

WeatherForecastManager::WeatherForecastManager(WeatherLocationListModel &model)
    : model_(model)
{
    // create cache location if it does not exist, and load cache
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/cache");
    if (!dir.exists())
        dir.mkpath(".");
    readFromCache();
}

WeatherForecastManager &WeatherForecastManager::instance(WeatherLocationListModel &model)
{
    static WeatherForecastManager singleton(model);
    return singleton;
}
void WeatherForecastManager::update()
{
    qDebug() << "update start";
    auto locations = model_.getVec();
    for (auto wLocation : locations) {
        wLocation->update();
    }
}

void WeatherForecastManager::readFromCache()
{
    QHash<WeatherLocation *, QExplicitlySharedDataPointer<KWeatherCore ::WeatherForecast>> map;

    QFile reader;
    QDirIterator iterator(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/cache", QDirIterator::Subdirectories); // list directory entries

    while (iterator.hasNext()) {
        reader.setFileName(iterator.next());
        QFileInfo fileName(reader); // strip absolute path
        bool isFound = false;       // indicate should we load this cache

        // loop over existing locations and add cached weather forecast data if location found
        for (auto wl : model_.getVec()) {
            if (fileName.fileName() == wl->locationId()) {
                isFound = true;
                reader.open(QIODevice::ReadOnly);
                auto fc = KWeatherCore::WeatherForecast::fromJson(QJsonDocument::fromJson(reader.readAll()).object());

                map[wl] = fc;
                break;
            }
        }

        if (!isFound) { // delete no longer needed cache
            reader.remove();
        }
        reader.close();
    }

    // add to loaded locations from cache
    for (auto wl : model_.getVec()) {
        if (map.find(wl) != map.end()) { // is in cache
            wl->initData(map[wl]);
        }
    }

    update();
}
