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
public slots:
    void refreshHoursFromForecasts(QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> forecast);

protected:
    friend class WeatherLocation;
    WeatherHour *currentForecast() const;

private:
    QVector<WeatherHour *> hoursVec;
    QVector<int> dayVec;
    int day = 0;
};
