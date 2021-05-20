/*
    SPDX-FileCopyrightText: 2020 HanY <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "kweather_1x4.h"
#include "locationmodel.h"
#include <KWeatherCore/WeatherForecastSource>
#include <QJsonArray>
#include <QJsonDocument>
#include <QLocale>
#include <QTimer>
KWeather_1x4::KWeather_1x4(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
    , m_model(new LocationModel())
{
}
K_EXPORT_PLASMA_APPLET_WITH_JSON(kweather_1x4, KWeather_1x4, "metadata.json")

#include "kweather_1x4.moc"
