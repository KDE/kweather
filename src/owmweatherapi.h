/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef OPENWEATHERMAP_H
#define OPENWEATHERMAP_H
#include "abstractweatherapi.h"
#include <QObject>

// OpenWeatherMap API Implementation
// api.openweathermap.org
class OWMWeatherAPI : public AbstractWeatherAPI
{
    Q_OBJECT

public:
    OWMWeatherAPI(QString locationName);
    ~OWMWeatherAPI();
    void setToken(QString &token) override;
    void setLocation(float latitude, float longitude) override;
    void update() override;

private slots:

    void parse(QNetworkReply *Reply) override;

private:
    QHash<QDate, AbstractDailyWeatherForecast *> dayCache;
    // rank weather (for what best describes the day overall)
    static const QHash<QString, int> rank;
    static const QHash<QString, QString> map;
};
#endif // OPENWEATHERMAP_H
