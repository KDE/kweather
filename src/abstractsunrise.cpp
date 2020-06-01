/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "abstractsunrise.h"
#include <QJsonObject>
AbstractSunrise::AbstractSunrise()
{
}

AbstractSunrise::AbstractSunrise(QDateTime &sunRise,
                                 QDateTime &sunSet,
                                 QDateTime &moonRise,
                                 QDateTime &moonSet,
                                 QPair<QDateTime, double> &highMoon,
                                 QPair<QDateTime, double> &lowMoon,
                                 QPair<QDateTime, double> &solarMidnight,
                                 QPair<QDateTime, double> &solarNoon,
                                 double moonPhase)
{
    this->sunRise_ = sunRise.isValid() ? sunRise : QDateTime::currentDateTime();
    this->sunSet_ = sunSet.isValid() ? sunSet : QDateTime::currentDateTime();
    this->moonRise_ = moonRise.isValid() ? moonRise : QDateTime::currentDateTime();
    this->moonSet_ = moonSet.isValid() ? moonSet : QDateTime::currentDateTime();
    this->highMoon_ = highMoon;
    this->lowMoon_ = lowMoon;
    this->solarMidnight_ = solarMidnight;
    this->solarNoon_ = solarNoon;
    this->moonPhase_ = moonPhase;
}

QJsonObject AbstractSunrise::toJson()
{
    QJsonObject obj;
    obj[QLatin1String("sunrise")] = sunRise_.toString(Qt::ISODate);
    obj[QLatin1String("sunset")] = sunSet_.toString(Qt::ISODate);
    obj[QLatin1String("moonrise")] = moonRise_.toString(Qt::ISODate);
    obj[QLatin1String("moonset")] = moonSet_.toString(Qt::ISODate);
    obj[QLatin1String("lowmoon")] = lowMoon_.first.toString(Qt::ISODate);
    obj[QLatin1String("solarmidnight")] = solarMidnight_.first.toString(Qt::ISODate);
    obj[QLatin1String("solarnoon")] = solarNoon_.first.toString(Qt::ISODate);
    obj[QLatin1String("highmoon")] = highMoon_.first.toString(Qt::ISODate);
    obj[QLatin1String("lowmoonEle")] = lowMoon_.second;
    obj[QLatin1String("highmoonEle")] = highMoon_.second;
    obj[QLatin1String("solarmidnightEle")] = solarMidnight_.second;
    obj[QLatin1String("solarNoonEle")] = solarNoon_.second;
    obj[QLatin1String("moonphase")] = moonPhase_;
    return obj;
}

AbstractSunrise AbstractSunrise::fromJson(QJsonObject obj)
{
    AbstractSunrise as;
    as.setSunSet(QDateTime::fromString(obj["sunset"].toString(), Qt::ISODate));
    as.setSunRise(QDateTime::fromString(obj["sunrise"].toString(), Qt::ISODate));
    as.setLowMoon(QPair<QDateTime, double>(QDateTime::fromString(obj["lowmoon"].toString(), Qt::ISODate), obj["lowmoonEle"].toDouble()));
    as.setHighMoon(QPair<QDateTime, double>(QDateTime::fromString(obj["highmoon"].toString(), Qt::ISODate), obj["highmoonEle"].toDouble()));
    as.setSolarMidnight(QPair<QDateTime, double>(QDateTime::fromString(obj["solarmidnight"].toString(), Qt::ISODate), obj["solarmidnightEle"].toDouble()));
    as.setSolarNoon(QPair<QDateTime, double>(QDateTime::fromString(obj["solarnoon"].toString(), Qt::ISODate), obj["solarnoonEle"].toDouble()));
    as.setMoonPhase(obj["moonphase"].toDouble());
    as.setMoonSet(QDateTime::fromString(obj["moonset"].toString(), Qt::ISODate));
    as.setMoonRise(QDateTime::fromString(obj["moonrise"].toString(), Qt::ISODate));
    return as;
}
