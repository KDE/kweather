/*
    SPDX-FileCopyrightText: 2020 HanY <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef KWEATHER_1X4_H
#define KWEATHER_1X4_H
#include <KWeatherCore/WeatherForecast>
#include <Plasma/Applet>
#include <QAbstractListModel>
#include <vector>
namespace KWeatherCore
{
class PendingWeatherForecast;
}
class QTimer;
class LocationModel;
class KWeather_1x4 : public Plasma::Applet
{
    Q_OBJECT
    Q_PROPERTY(LocationModel *locationModel READ locationModel NOTIFY locationModelChanged)
public:
    KWeather_1x4(QObject *parent, const QVariantList &args);
    LocationModel *locationModel() {
        return m_model;
    }
signals:
    void locationModelChanged();
private slots:
    void update();

private:
    LocationModel *m_model = nullptr;
};

#endif
