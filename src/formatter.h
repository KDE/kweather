/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QDateTime>
#include <QObject>
#include <qqmlregistration.h>

class Formatter : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    Q_INVOKABLE double convertTemp(qreal temperature, const QString &unit) const;
    Q_INVOKABLE QString formatTemperatureUnitDegrees(const QString &unit) const;
    Q_INVOKABLE QString formatTemperature(qreal temperature, const QString &unit) const;
    Q_INVOKABLE QString formatTemperatureRounded(qreal temperature, const QString &unit) const;

    Q_INVOKABLE QString formatWindSpeed(qreal speed, const QString &unit) const;

    Q_INVOKABLE QString formatPressure(qreal pressure, const QString &unit) const;
    Q_INVOKABLE QString formatPercent(qreal percentage) const;
    Q_INVOKABLE QString formatDouble(qreal number) const;

    Q_INVOKABLE QString formatSunriseTime(QDateTime date, const QString &timeZone) const;
    Q_INVOKABLE QString formatPrecipitation(qreal precipitation, const QString &unit) const;
    Q_INVOKABLE QString formatHourlyCardDelegateTime(QDateTime date, const QString &timeZone) const;
};
