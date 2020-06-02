/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef ABSTRACTAPI_H
#define ABSTRACTAPI_H

#include "abstractweatherforecast.h"
#include "nmisunriseapi.h"
#include <QObject>
#include <memory>
#include <utility>
#include <vector>

class QNetworkAccessManager;
class QNetworkReply;
class AbstractDailyWeatherForecast;
class AbstractWeatherAPI : public QObject
{
    Q_OBJECT

public:
    const int interval = -1; // api update interval in hour

    AbstractWeatherAPI(QString locationId, QString timeZone, int interval, double latitude, double longitude, QObject *parent = nullptr);
    ~AbstractWeatherAPI() override;

    virtual void update() = 0;
    virtual void applySunriseDataToForecast() = 0;

    // MUST BE CALLED if not loading sunrise data from cache
    void fetchSunriseData(); // call api for sunrise/sunset data

    AbstractWeatherForecast& currentData();
    void setCurrentData(AbstractWeatherForecast forecast);
    QList<AbstractSunrise> currentSunriseData();
    void setCurrentSunriseData(QList<AbstractSunrise> currentSunriseData);
    void setLocation(float lat, float lon);
    QString &timeZone();
    Kweather::WindDirection getWindDirect(double deg);

protected:
    QString locationId_;
    QString timeZone_;
    float latitude_, longitude_;

    QNetworkAccessManager *mManager;
    QNetworkReply *mReply;

    AbstractWeatherForecast currentData_;
    QList<AbstractSunrise> currentSunriseData_;

    NMISunriseAPI* sunriseApi_;

signals:
    void updated(AbstractWeatherForecast& forecast);
    void networkError();
public slots:
    virtual void parse(QNetworkReply *Reply) = 0;
};

#endif // ABSTRACTAPI_H
