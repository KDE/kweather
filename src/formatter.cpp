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
    return KWeather::convertTemp(temperature, unit);
}

QString Formatter::formatTemperatureUnitDegrees(const QString &unit) const
{
    if (KWeather::isCelsius(unit)) {
        return QStringLiteral("℃");
    } else {
        return QStringLiteral("℉");
    }
}

QString Formatter::formatTemperature(qreal temperature, const QString &unit) const
{
    // only have decimals when in celsius
    if (KWeather::isCelsius(unit)) {
        QString formattedTemperature = QLocale().toString(KWeather::convertTemp(temperature, unit), 'f', 1);
        return ki18nc("A temperature", "%1°").subs(formattedTemperature).toString();
    } else {
        return formatTemperatureRounded(temperature, unit);
    }
}

QString Formatter::formatTemperatureRounded(qreal temperature, const QString &unit) const
{
    return i18nc("A temperature", "%1°", qRound(KWeather::convertTemp(temperature, unit)));
}

QString Formatter::formatWindSpeed(qreal speed, const QString &unit) const
{
    QString formattedSpeed;

    if (unit == QLatin1String("kph")) {
        formattedSpeed = QLocale().toString(speed, 'f', 1);
        return ki18n("%1 km/h").subs(formattedSpeed).toString();
    } else if (unit == QLatin1String("mph")) {
        formattedSpeed = QLocale().toString(speed * 0.62, 'f', 1);
        return ki18n("%1 mph").subs(formattedSpeed).toString();
    } else {
        formattedSpeed = QLocale().toString(speed * 1000 / 3600, 'f', 1);
        return ki18n("%1 m/s").subs(formattedSpeed).toString();
    }
}

QString Formatter::formatPressure(qreal pressure, const QString &unit) const
{
    QString formattedPressure;

    if (unit == QLatin1String("hPa")) {
        formattedPressure = QLocale().toString(pressure, 'f', 1);
        return ki18n("%1 hPa").subs(formattedPressure).toString();
    } else {
        formattedPressure = QLocale().toString(pressure * 0.7500638, 'f', 1);
        return ki18n("%1 mmHg").subs(formattedPressure).toString();
    }
}

QString Formatter::formatPercent(qreal percentage) const
{
    return i18nc("%1 represents percent value, % is the percent sign", "%1%", percentage);
}

QString Formatter::formatDouble(qreal number) const
{
    return i18n("%1", QLocale().toString(number));
}

QString Formatter::formatSunriseTime(QDateTime date, const QString &timeZone) const
{
    if (!date.isValid()) {
        return i18nc("sunrise time not available", "-");
    }
    return QLocale().toString(date.toTimeZone(QTimeZone(timeZone.toUtf8())).time(), QLocale::ShortFormat).toLower();
}

QString Formatter::formatPrecipitation(qreal precipitation, const QString &unit) const
{
    QString formattedPrecipitation;

    if (unit == QStringLiteral("in")) {
        formattedPrecipitation = QLocale().toString(precipitation * 0.03937008, 'f', 2);
        return ki18nc("in as inches", "%1 in").subs(formattedPrecipitation).toString();
    } else {
        formattedPrecipitation = QLocale().toString(precipitation, 'f', 1);
        return ki18nc("mm as millimeters", "%1 mm").subs(formattedPrecipitation).toString();
    }
}

QString Formatter::formatHourlyCardDelegateTime(QDateTime date, const QString &timeZone) const
{
    return QLocale().toString(date.toTimeZone(QTimeZone(timeZone.toUtf8())).time(), QLocale::ShortFormat).toLower();
}

#include "moc_formatter.cpp"
