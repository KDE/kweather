/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef KWEATHER_SETTINGSMODEL_H
#define KWEATHER_SETTINGSMODEL_H

#include <QObject>
#include <QSettings>
#include <global.h>
class SettingsModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString temperatureUnits READ temperatureUnits WRITE setTemperatureUnits NOTIFY propertyChanged)
    Q_PROPERTY(QString speedUnits READ speedUnits WRITE setSpeedUnits NOTIFY propertyChanged)
    Q_PROPERTY(QString OWMToken READ OWMToken WRITE setOWMToken NOTIFY propertyChanged)
    Q_PROPERTY(QString defaultBackend READ defaultBackend WRITE setDefaultBackend NOTIFY propertyChanged)

public:
    inline QString temperatureUnits()
    {
        QSettings settings;
        return settings.value("Global/temperatureUnits", "Celsius").toString();
    }

    inline void setTemperatureUnits(QString temp)
    {
        QSettings settings;
        settings.setValue("Global/temperatureUnits", temp);
        emit propertyChanged();
    }

    inline QString speedUnits()
    {
        QSettings settings;
        return settings.value("Global/speedUnits", "kph").toString();
    }

    inline void setSpeedUnits(QString speed)
    {
        QSettings settings;
        settings.setValue("Global/speedUnits", speed);
        emit propertyChanged();
    }

    inline void setOWMToken(QString token)
    {
        QSettings settings;
        settings.setValue("Global/OWMToken", token);
        emit propertyChanged();
    }

    inline QString OWMToken()
    {
        QSettings settings;
        return settings.value("Global/OWMToken").toString();
    }

    inline QString defaultBackend()
    {
        QSettings settings;
        return settings.value("Global/defaultBackend", Kweather::API_NMI).toString();
    }

    inline void setDefaultBackend(QString backend)
    {
        QSettings settings;
        settings.setValue("Global/defaultBackend", backend);
        emit propertyChanged();
    }
signals:
    void propertyChanged();
};

#endif // KWEATHER_SETTINGSMODEL_H
