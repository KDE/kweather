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
class HourlyModel;
class KWeather_1x4 : public Plasma::Applet
{
    Q_OBJECT
    Q_PROPERTY(LocationModel *locationModel READ locationModel NOTIFY locationModelChanged)
    Q_PROPERTY(HourlyModel *hourlyModel READ hourlyModel NOTIFY hourlyModelChanged)
public:
    KWeather_1x4(QObject *parent, const QVariantList &args);
    LocationModel *locationModel() {
        return m_locationModel;
    }
    HourlyModel *hourlyModel() {
        return m_hourlyModel;
    }
    Q_INVOKABLE void setDetailed(int index);
signals:
    void locationModelChanged();
    void hourlyModelChanged();
private:
    LocationModel *m_locationModel = nullptr;
    HourlyModel *m_hourlyModel = nullptr;
};

#endif
