#ifndef ABSTRACTSUNRISE_H
#define ABSTRACTSUNRISE_H

#include <QDateTime>
#include <QObject>
#include <QPair>
class AbstractSunrise : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString highMoonTime READ highMoonTime NOTIFY propertyChanged)
    Q_PROPERTY(QString lowMoonTime READ lowMoonTime NOTIFY propertyChanged)
    Q_PROPERTY(QString solarMidnightTime READ solarMidnightTime NOTIFY propertyChanged)
    Q_PROPERTY(QString solarNoonTime READ solarNoonTime NOTIFY propertyChanged)
    Q_PROPERTY(QString sunRiseStr READ sunRiseStr NOTIFY propertyChanged)
    Q_PROPERTY(QString sunSetStr READ sunSetStr NOTIFY propertyChanged)
    Q_PROPERTY(QString moonRiseStr READ moonRiseStr NOTIFY propertyChanged)
    Q_PROPERTY(QString moonSetStr READ moonSetStr NOTIFY propertyChanged)
    Q_PROPERTY(double highMoon READ highMoon NOTIFY propertyChanged)
    Q_PROPERTY(double lowMoon READ lowMoon NOTIFY propertyChanged)
    Q_PROPERTY(double solarMidnight READ solarMidnight NOTIFY propertyChanged)
    Q_PROPERTY(double solarNoon READ solarNoon NOTIFY propertyChanged)
public:
    AbstractSunrise(QDateTime &sunRise,
                    QDateTime &sunSet,
                    QDateTime &moonRise,
                    QDateTime &moonSet,
                    QPair<QDateTime, double> &highMoon,
                    QPair<QDateTime, double> &lowMoon,
                    QPair<QDateTime, double> &solarMidnight,
                    QPair<QDateTime, double> &solarNoon);
    AbstractSunrise();
    AbstractSunrise *fromJson(QJsonObject obj);
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
    QDateTime &sunRise()
    {
        return sunRise_;
    };
    QDateTime &sunSet()
    {
        return sunSet_;
    };
    QDateTime &moonRise()
    {
        return moonRise_;
    };
    QDateTime &moonSet()
    {
        return moonSet_;
    };
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
signals:
    void propertyChanged();

private:
    QPair<QDateTime, double> highMoon_;
    QPair<QDateTime, double> solarMidnight_;
    QPair<QDateTime, double> solarNoon_;
    QPair<QDateTime, double> lowMoon_;
    QDateTime sunRise_;
    QDateTime sunSet_;
    QDateTime moonRise_;
    QDateTime moonSet_;
};

#endif // ABSTRACTSUNRISE_H
