/*
    SPDX-FileCopyrightText: 2020 HanY <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "kweather_1x4.h"
#include "locationmodel.h"
#include "hourlymodel.h"
#include <KWeatherCore/WeatherForecastSource>
#include <QQmlApplicationEngine>
KWeather_1x4::KWeather_1x4(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
    , m_locationModel(new LocationModel())
    , m_hourlyModel(new HourlyModel())
{
    m_hourlyModel->setLocationModel(m_locationModel);
    qmlRegisterAnonymousType<LocationModel>("LocationModel", 1);
    qmlRegisterAnonymousType<HourlyModel>("HourlyModel", 1);
}
void KWeather_1x4::setDetailed(int index)
{
    m_hourlyModel->loadForecast(m_locationModel->getData(index), index);
}
K_EXPORT_PLASMA_APPLET_WITH_JSON(kweather_1x4, KWeather_1x4, "metadata.json")

#include "kweather_1x4.moc"
