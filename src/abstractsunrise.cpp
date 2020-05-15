#include "abstractsunrise.h"
#include <QJsonObject>
AbstractSunrise::AbstractSunrise()
{
}

AbstractSunrise::AbstractSunrise(QDateTime &sunRise, QDateTime &sunSet, QDateTime &moonRise, QDateTime &moonSet, QPair<QDateTime, double> &highMoon, QPair<QDateTime, double> &lowMoon, QPair<QDateTime, double> &solarMidnight)
    : sunRise_(sunRise)
    , sunSet_(sunSet)
    , moonRise_(moonRise)
    , moonSet_(moonSet)
    , highMoon_(highMoon)
    , lowMoon_(lowMoon)
    , solarMidnight_(solarMidnight)
{
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
    obj[QLatin1String("highmoon")] = highMoon_.first.toString(Qt::ISODate);
    obj[QLatin1String("lowmoonEle")] = lowMoon_.second;
    obj[QLatin1String("highmoonEle")] = highMoon_.second;
    obj[QLatin1String("solarmidnightEle")] = solarMidnight_.second;
    return obj;
}

AbstractSunrise *AbstractSunrise::fromJson(QJsonObject obj)
{
    auto as = new AbstractSunrise();
    as->setSunSet(QDateTime::fromString(obj["sunrise"].toString(), Qt::ISODate));
    as->setSunRise(QDateTime::fromString(obj["sunrise"].toString(), Qt::ISODate));
    as->setLowMoon(QPair<QDateTime, double>(QDateTime::fromString(obj["lowmoon"].toString(), Qt::ISODate), obj["lowmoonEle"].toDouble()));
    as->setHighMoon(QPair<QDateTime, double>(QDateTime::fromString(obj["highmoon"].toString(), Qt::ISODate), obj["highmoonEle"].toDouble()));
    as->setSolarMidnight(QPair<QDateTime, double>(QDateTime::fromString(obj["solarmidnight"].toString(), Qt::ISODate), obj["solarmidnightEle"].toDouble()));
    return as;
}
