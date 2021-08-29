/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QObject>

class Formatter : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE QString formatTemperature(qreal temperature, const QString &unit) const;
    Q_INVOKABLE QString formatTemperatureRounded(qreal temperature, const QString &unit) const;

    Q_INVOKABLE QString formatWindSpeed(qreal speed, const QString &unit) const;
};
