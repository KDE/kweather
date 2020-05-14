#include "abstractsunrise.h"

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
