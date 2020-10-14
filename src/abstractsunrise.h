/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#ifndef ABSTRACTSUNRISE_H
#define ABSTRACTSUNRISE_H

#include <QDateTime>
#include <QObject>
#include <QPair>
class AbstractSunrise
{
public:
    AbstractSunrise(QDateTime &sunRise,
                    QDateTime &sunSet,
                    QDateTime &moonRise,
                    QDateTime &moonSet,
                    QPair<QDateTime, double> &highMoon,
                    QPair<QDateTime, double> &lowMoon,
                    QPair<QDateTime, double> &solarMidnight,
                    QPair<QDateTime, double> &solarNoon,
                    double moonphase);
    AbstractSunrise();
    static AbstractSunrise fromJson(QJsonObject obj);
    QJsonObject toJson();
    QString highMoonTime() const
    {
        return highMoon_.first.time().toString();
    };
    double highMoon()
    {
        return highMoon_.second;
    };
    QString lowMoonTime() const
    {
        return lowMoon_.first.time().toString();
    };
    double lowMoon()
    {
        return lowMoon_.second;
    };
    QString solarMidnightTime() const
    {
        return solarMidnight_.first.time().toString();
    };
    QString solarNoonTime() const
    {
        return solarNoon_.first.time().toString();
    };
    double solarMidnight()
    {
        return solarMidnight_.second;
    };
    double solarNoon()
    {
        return solarNoon_.second;
    };
    const QDateTime &sunRise() const
    {
        return sunRise_;
    };
    const QDateTime &sunSet() const
    {
        return sunSet_;
    };
    const QDateTime &moonRise() const
    {
        return moonRise_;
    };
    const QDateTime &moonSet() const
    {
        return moonSet_;
    };
    double moonPhase()
    {
        return moonPhase_;
    }
    QString sunRiseStr() const
    {
        return sunRise_.time().toString();
    };
    QString sunSetStr() const
    {
        return sunSet_.time().toString();
    };
    QString moonRiseStr() const
    {
        return moonRise_.time().toString();
    };
    QString moonSetStr() const
    {
        return moonSet_.time().toString();
    };
    void setHighMoon(QPair<QDateTime, double> highMoon)
    {
        highMoon_ = highMoon;
    };
    void setSolarMidnight(QPair<QDateTime, double> solarMidnight)
    {
        solarMidnight_ = solarMidnight;
    };
    void setSolarNoon(QPair<QDateTime, double> solarNoon)
    {
        solarNoon_ = solarNoon;
    };
    void setLowMoon(QPair<QDateTime, double> lowMoon)
    {
        lowMoon_ = lowMoon;
    };
    void setSunRise(QDateTime sunRise)
    {
        sunRise_ = sunRise;
    };
    void setSunSet(QDateTime sunSet)
    {
        sunSet_ = sunSet;
    };
    void setMoonRise(QDateTime moonRise)
    {
        moonRise_ = moonRise;
    };
    void setMoonSet(QDateTime moonSet)
    {
        moonSet_ = moonSet;
    };
    void setMoonPhase(double moonPhase)
    {
        moonPhase_ = moonPhase;
    }

private:
    QPair<QDateTime, double> highMoon_;
    QPair<QDateTime, double> solarMidnight_;
    QPair<QDateTime, double> solarNoon_;
    QPair<QDateTime, double> lowMoon_;
    QDateTime sunRise_ = QDateTime::currentDateTime();
    QDateTime sunSet_ = QDateTime::currentDateTime();
    QDateTime moonRise_ = QDateTime::currentDateTime();
    QDateTime moonSet_ = QDateTime::currentDateTime();
    double moonPhase_;
};

#endif // ABSTRACTSUNRISE_H
