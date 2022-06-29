/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "formatter.h"

#include <KLocalizedString>
#include <QString>
#include <QTimeZone>

#include "global.h"

double Formatter::convertTemp(qreal temperature, const QString &unit) const
{
    return Kweather::convertTemp(temperature, unit);
}

QString Formatter::formatTemperatureUnitDegrees(const QString &unit) const
{
    if (unit == QLatin1String("Celsius") || (unit == QLatin1String("Use System Default") && QLocale().measurementSystem() == QLocale::MetricSystem)) {
        return QStringLiteral("℃");
    } else {
        return QStringLiteral("℉");
    }
}

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
        return i18n("%1 km/h", QString::number(speed, 'f', 1));
    } else if (unit == QLatin1String("mph")) {
        return i18n("%1 mph", QString::number(speed * 0.62, 'f', 1));
    } else {
        return i18n("%1 m/s", QString::number(speed * 1000 / 3600, 'f', 1));
    }
}

QString Formatter::formatPressure(qreal pressure, const QString &unit) const
{
    if (unit == QLatin1String("hPa")) {
        return i18n("%1 hPa", QString::number(pressure, 'f', 1));
    } else {
        return i18n("%1 mmHg", QString::number(pressure * 0.7500638, 'f', 1));
    }
}

QString Formatter::formatSunriseTime(QDateTime date, const QString &timeZone) const
{
    if (!date.isValid()) {
        return i18nc("sunrise time not available", "-");
    }
    return date.toTimeZone(QTimeZone(timeZone.toUtf8())).toString(QStringLiteral("hh:mm ap"));
}

QString Formatter::formatHourlyCardDelegateTime(QDateTime date, const QString &timeZone) const
{
    return date.toTimeZone(QTimeZone(timeZone.toUtf8())).toString(QStringLiteral("h ap")).replace(QStringLiteral("."), QStringLiteral(""));
}
