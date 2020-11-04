/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include <KWeatherCore/DailyForecast>
#include <QObject>
class WeatherDay : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString weatherDescription READ weatherDescription NOTIFY propertyChanged)
    Q_PROPERTY(QString weatherIcon READ weatherIcon NOTIFY propertyChanged)
    Q_PROPERTY(QDate date READ date NOTIFY propertyChanged)
    Q_PROPERTY(QString maxTemp READ maxTemp NOTIFY propertyChanged)
    Q_PROPERTY(QString minTemp READ minTemp NOTIFY propertyChanged)
    Q_PROPERTY(float precipitation READ precipitation NOTIFY propertyChanged)
    Q_PROPERTY(float uvIndex READ uvIndex NOTIFY propertyChanged)
    Q_PROPERTY(float humidity READ humidity NOTIFY propertyChanged)
    Q_PROPERTY(float pressure READ pressure NOTIFY propertyChanged)
    Q_PROPERTY(QString sunrise READ sunrise NOTIFY propertyChanged)
    Q_PROPERTY(QString sunset READ sunset NOTIFY propertyChanged)
    Q_PROPERTY(QString moonPhase READ moonPhase NOTIFY propertyChanged)

public:
    explicit WeatherDay();
    // list of forecasts on this day
    explicit WeatherDay(const KWeatherCore::DailyWeatherForecast &dailyForecast);

    const QString &weatherDescription() const;
    const QString &weatherIcon() const;
    const QDate &date() const;
    QString maxTemp() const;
    QString minTemp() const;
    float precipitation() const;
    float uvIndex() const;
    float humidity() const;
    float pressure() const;
    const QString &sunrise() const;
    const QString &sunset() const;
    const QString &moonPhase() const;

Q_SIGNALS:
    void propertyChanged();

private:
    QString m_weatherDescription;
    QString m_weatherIcon;
    QDate m_date;
    float m_maxTemp;
    float m_minTemp;
    float m_precipitation;
    float m_uvIndex;
    float m_humidity;
    float m_pressure;

    QString m_sunrise;
    QString m_sunset;
    QString m_moonPhase;
};
