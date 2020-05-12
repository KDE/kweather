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
