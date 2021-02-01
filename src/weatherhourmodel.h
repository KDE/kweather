/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#pragma once

#include "weatherhour.h"
#include <KWeatherCore/WeatherForecast>
#include <QAbstractListModel>
#include <QObject>

class WeatherLocation;
class WeatherHour;
class WeatherHourListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit WeatherHourListModel(WeatherLocation *location = nullptr);

    enum Roles { HourItemRole = Qt::UserRole };

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE WeatherHour *get(int index);

    Q_INVOKABLE void updateHourView(int index);
    Q_INVOKABLE void updateUi();
public Q_SLOTS:
    void refreshHoursFromForecasts(QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> forecast);
Q_SIGNALS:
    void weatherRefresh(SharedForecastPtr ptr);

protected:
    friend class WeatherLocation;
    WeatherHour *currentForecast() const;

private:
    std::vector<std::vector<WeatherHour *>> m_hours;
    int m_currentDay = 0;
};
