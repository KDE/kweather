/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef NMIWEATHERAPI2_H
#define NMIWEATHERAPI2_H

#include "abstractweatherapi.h"
#include "abstractweatherforecast.h"
#include <QObject>
// Norwegian Meteorological Institute Weather API Implementation (v2)
// api.met.no

class NMIWeatherAPI2 : public AbstractWeatherAPI
{
    Q_OBJECT

public:
    NMIWeatherAPI2(QString locationId);
    ~NMIWeatherAPI2() override;
    void update() override;
    void setLocation(float lat, float lon) override;
    void setToken(QString &) override;
signals:
    void noTimeZone();
private slots:
    void parse(QNetworkReply *Reply) override;

private:
    void parseOneElement(QJsonObject &object, QHash<QDate, AbstractDailyWeatherForecast *> &dayCache, QList<AbstractHourlyWeatherForecast *> &hoursList);
};

#endif
