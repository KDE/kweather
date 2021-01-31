/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include <KWeatherCore/DailyWeatherForecast>
#include <QObject>
class WeatherDay : public KWeatherCore::DailyWeatherForecast
{
    Q_GADGET
    Q_PROPERTY(QString sunrise READ sunrise)
    Q_PROPERTY(QString sunset READ sunset)
    Q_PROPERTY(QString moonPhase READ moonPhase)

public:
    WeatherDay();
    explicit WeatherDay(const KWeatherCore::DailyWeatherForecast &dailyForecast);
    const QString &sunrise() const;
    const QString &sunset() const;
    const QString &moonPhase() const;

private:
    QString m_sunrise;
    QString m_sunset;
    QString m_moonPhase;
};
Q_DECLARE_METATYPE(WeatherDay)
