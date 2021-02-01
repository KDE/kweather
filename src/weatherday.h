/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include "global.h"
#include <KWeatherCore/WeatherForecast>
#include <QObject>
using SharedForecastPtr = QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast>;
class WeatherDayListModel;
class WeatherDay : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString weatherDescription READ weatherDescription NOTIFY propertyChanged)
    Q_PROPERTY(QString weatherIcon READ weatherIcon NOTIFY propertyChanged)
    Q_PROPERTY(QDateTime date READ date NOTIFY propertyChanged)
    Q_PROPERTY(QString maxTemp READ maxTemp NOTIFY propertyChanged)
    Q_PROPERTY(QString minTemp READ minTemp NOTIFY propertyChanged)
    Q_PROPERTY(qreal precipitation READ precipitation NOTIFY propertyChanged)
    Q_PROPERTY(qreal uvIndex READ uvIndex NOTIFY propertyChanged)
    Q_PROPERTY(qreal humidity READ humidity NOTIFY propertyChanged)
    Q_PROPERTY(qreal pressure READ pressure NOTIFY propertyChanged)

    Q_PROPERTY(QString sunrise READ sunrise NOTIFY propertyChanged)
    Q_PROPERTY(QString sunset READ sunset NOTIFY propertyChanged)
    Q_PROPERTY(QString moonPhase READ moonPhase NOTIFY propertyChanged)

public:
    WeatherDay(WeatherDayListModel *parent = nullptr);
    explicit WeatherDay(SharedForecastPtr ptr, int day, WeatherDayListModel *parent = nullptr);
    const QString &weatherDescription() const
    {
        return day().weatherDescription();
    }
    const QString &weatherIcon() const
    {
        return day().weatherIcon();
    }
    QDateTime date() const
    {
        return day().dateTime();
    }
    QString maxTemp() const
    {
        return Kweather::tempConverter(day().maxTemp());
    }
    QString minTemp() const
    {
        return Kweather::tempConverter(day().minTemp());
    }
    qreal precipitation() const
    {
        return day().precipitation();
    }
    qreal uvIndex() const
    {
        return day().uvIndex();
    }
    qreal humidity() const
    {
        return day().humidity();
    }
    qreal pressure() const
    {
        return day().pressure();
    }
    const QString &sunrise() const
    {
        return m_sunrise;
    }
    const QString &sunset() const
    {
        return m_sunset;
    }
    const QString &moonPhase() const
    {
        return m_moonPhase;
    }

public Q_SLOTS:
       void refreshDaysFromForecasts(SharedForecastPtr ptr);

Q_SIGNALS:
       void propertyChanged();
private:

    const KWeatherCore::DailyWeatherForecast &day() const
    {
        return m_forecast->dailyWeatherForecast().at(m_day);
    }

    void determineSunrise();

    QString m_sunrise;
    QString m_sunset;
    QString m_moonPhase;
    int m_day = 0;
    SharedForecastPtr m_forecast;
};
