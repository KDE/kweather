/*
    SPDX-FileCopyrightText: 2020 HanY <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "kweather_1x4.h"
#include <QDBusInterface>

KWeather_1x4::KWeather_1x4(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
{
    QDBusConnection::sessionBus().connect("org.kde.kweather", "/", "org.kde.kweather.LocationModel", "removed", this, SLOT(addCity(QString)));
    QDBusConnection::sessionBus().connect("org.kde.kweather", "/", "org.kde.kweather.LocationModel", "added", this, SLOT(removeCity(QString)));
}

K_EXPORT_PLASMA_APPLET_WITH_JSON(kweather_1x4, KWeather_1x4, "metadata.json")

#include "kweather_1x4.moc"
