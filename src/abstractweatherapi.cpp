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
#include <QTimeZone>

AbstractWeatherAPI::AbstractWeatherAPI(QString locationId, QString timeZone, int interval, double latitude, double longitude, QObject *parent)
    : QObject(parent)
    , locationId_(std::move(locationId))
    , timeZone_(std::move(timeZone))
    , interval(interval)
    , latitude_(latitude)
    , longitude_(longitude)
{
    mManager = new QNetworkAccessManager();
    sunriseApi_ = new NMISunriseAPI(latitude, longitude, QDateTime::currentDateTime().toTimeZone(QTimeZone(QByteArray::fromStdString(timeZone_.toStdString()))).offsetFromUtc());
    connect(sunriseApi_, &NMISunriseAPI::finished, this, [this]() {
        qDebug() << "obtained sunrise data " << sunriseApi_->get().count();

        setCurrentSunriseData(sunriseApi_->get());
        applySunriseDataToForecast();
        if (!currentData_.hourlyForecasts().empty())
            emit updated(currentData_); // update ui
    });

    sunriseApi_->update();
}

AbstractWeatherAPI::~AbstractWeatherAPI()
{
    delete mManager;
    delete sunriseApi_;
}

QString &AbstractWeatherAPI::timeZone()
{
    return timeZone_;
};
AbstractWeatherForecast& AbstractWeatherAPI::currentData()
{
    return currentData_;
}
void AbstractWeatherAPI::setCurrentData(AbstractWeatherForecast forecast)
{
    currentData_ = forecast;
}
void AbstractWeatherAPI::setLocation(float lat, float lon)
{
    latitude_ = lat;
    longitude_ = lon;
}
QList<AbstractSunrise> AbstractWeatherAPI::currentSunriseData()
{
    return currentSunriseData_;
}
void AbstractWeatherAPI::setCurrentSunriseData(QList<AbstractSunrise> sunrise)
{
    currentSunriseData_ = sunrise;
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