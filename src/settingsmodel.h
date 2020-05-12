/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef KWEATHER_SETTINGSMODEL_H
#define KWEATHER_SETTINGSMODEL_H

#include <QtCore/QObject>
#include <QtCore/QSettings>
class SettingsModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString temperatureUnits READ temperatureUnits WRITE setTemperatureUnits NOTIFY propertyChanged)

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

signals:
    void propertyChanged();
};

#endif // KWEATHER_SETTINGSMODEL_H
