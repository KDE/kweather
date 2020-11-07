/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef WEATHERFORECASTMANAGER_H
#define WEATHERFORECASTMANAGER_H

#include "global.h"
#include <KWeatherCore/WeatherForecast>
#include <QObject>

class AbstractWeatherForecast;
class NMIWeatherAPI2;
class WeatherLocationListModel;
class QTimer;
class WeatherLocation;
class WeatherForecastManager : public QObject
{
    Q_OBJECT

public:
    static WeatherForecastManager &instance(WeatherLocationListModel &model);

signals:
    void updated();
private slots:
    void update();

private:
    WeatherLocationListModel &model_;
    void readFromCache();
    WeatherForecastManager(WeatherLocationListModel &model);
};

#endif // WEATHERFORECASTMANAGER_H
