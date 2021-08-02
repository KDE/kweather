/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include "weatherday.h"
#include <KWeatherCore/WeatherForecast>
#include <QAbstractListModel>
#include <QObject>
class WeatherLocation;
class WeatherDay;
class WeatherDayListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit WeatherDayListModel(WeatherLocation *location = nullptr);

    enum Roles { DayItemRole = Qt::UserRole };

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE WeatherDay *get(int index);

    Q_INVOKABLE void updateUi();

public Q_SLOTS:
    void refreshDaysFromForecasts(KWeatherCore::WeatherForecast forecast);
Q_SIGNALS:
    void weatherRefresh(KWeatherCore::WeatherForecast forecast);

private:
    KWeatherCore::WeatherForecast m_forecasts;
    std::vector<WeatherDay *> m_weatherDays;
};
