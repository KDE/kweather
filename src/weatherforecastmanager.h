/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef WEATHERFORECASTMANAGER_H
#define WEATHERFORECASTMANAGER_H

#include "global.h"
#include <QObject>
#include <QRandomGenerator>
#include <vector>
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
    QRandomGenerator random;
    WeatherLocationListModel &model_;
    AbstractWeatherForecast convertFromJson(QByteArray data);
    QTimer *updateTimer = nullptr;
    void readFromCache();
    WeatherForecastManager(WeatherLocationListModel &model);
    WeatherForecastManager(const WeatherForecastManager &);
    WeatherForecastManager &operator=(const WeatherForecastManager &);
};

#endif // WEATHERFORECASTMANAGER_H
