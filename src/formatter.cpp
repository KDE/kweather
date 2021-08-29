/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "formatter.h"

#include <KLocalizedString>

#include "global.h"

QString Formatter::formatTemperature(qreal temperature, const QString &unit) const
{
    return i18nc("A temperature", "%1°", Kweather::convertTemp(temperature, unit));
}

QString Formatter::formatTemperatureRounded(qreal temperature, const QString &unit) const
{
    return i18nc("A temperature", "%1°", qRound(Kweather::convertTemp(temperature, unit)));
}

QString Formatter::formatWindSpeed(qreal speed, const QString &unit) const
{
    if (unit == QLatin1String("kph")) {
        return i18n("%1 km/h", QString::number(speed, 'g', 1));
    } else {
        return i18n("%1 mph", QString::number(speed * 0.62, 'g', 1));
    }
}
