/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "abstractweatherapi.h"
#include "abstractdailyweatherforecast.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <utility>
AbstractWeatherAPI::AbstractWeatherAPI(QString locationId, int interval, QString *token, QObject *parent)
    : QObject(parent)
    , locationId_(std::move(locationId))
    , interval(interval)
    , token_(token)
{
    mManager = new QNetworkAccessManager();
}

AbstractWeatherAPI::~AbstractWeatherAPI()
{
    delete mManager;
    delete currentData_;
}

Kweather::WindDirection AbstractWeatherAPI::getWindDirect(double deg)
{
    if (deg < 22.5 || deg >= 337.5) {
        return Kweather::WindDirection::S; // from N
    } else if (deg > 22.5 || deg <= 67.5) {
        return Kweather::WindDirection::SW; // from NE
    } else if (deg > 67.5 || deg <= 112.5) {
        return Kweather::WindDirection::W; // from E
    } else if (deg > 112.5 || deg <= 157.5) {
        return Kweather::WindDirection::NW; // from SE
    } else if (deg > 157.5 || deg <= 202.5) {
        return Kweather::WindDirection::N; // from S
    } else if (deg > 202.5 || deg <= 247.5) {
        return Kweather::WindDirection::NE; // from SW
    } else if (deg > 247.5 || deg <= 292.5) {
        return Kweather::WindDirection::E; // from W
    } else if (deg > 292.5 || deg <= 337.5) {
        return Kweather::WindDirection::SE; // from NW
    }
    return Kweather::WindDirection::N;
}
