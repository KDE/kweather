/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "weatherdaymodel.h"
#include "weatherday.h"
#include "weatherlocation.h"

WeatherDayListModel::WeatherDayListModel(WeatherLocation *location)
{
    connect(location, &WeatherLocation::weatherRefresh, this, &WeatherDayListModel::refreshDaysFromForecasts);
}

int WeatherDayListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return daysList.size();
}

QVariant WeatherDayListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= daysList.count() || index.row() < 0) {
        return {};
    }
    if (role == Roles::DayItemRole) {
        return QVariant::fromValue(daysList.at(index.row()));
    }
    return {};
}

QHash<int, QByteArray> WeatherDayListModel::roleNames() const
{
    return {{Roles::DayItemRole, "dayItem"}};
}

WeatherDay *WeatherDayListModel::get(int index)
{
    if (index < 0 || index >= daysList.count())
        return {};
    return daysList.at(index);
}

void WeatherDayListModel::refreshDaysFromForecasts(const KWeatherCore::WeatherForecast &forecasts)
{
    Q_EMIT layoutAboutToBeChanged();
    Q_EMIT beginRemoveRows(QModelIndex(), 0, daysList.count() - 1);
    auto oldList = daysList;
    daysList.clear();
    Q_EMIT endRemoveRows();

    Q_EMIT beginInsertRows(QModelIndex(), 0, forecasts.dailyWeatherForecast().count() - 1);

    // add weatherdays with forecast day lists
    for (auto forecast : forecasts.dailyWeatherForecast()) {
        WeatherDay *weatherDay = new WeatherDay(forecast);
        QQmlEngine::setObjectOwnership(weatherDay, QQmlEngine::CppOwnership); // prevent segfaults from js garbage collecting
        daysList.append(weatherDay);
    }

    Q_EMIT endInsertRows();
    Q_EMIT layoutChanged();
    for (auto ptr : oldList)
        ptr->deleteLater();
}

void WeatherDayListModel::updateUi()
{
    for (auto h : daysList) {
        Q_EMIT h->propertyChanged();
    }
    Q_EMIT dataChanged(createIndex(0, 0), createIndex(daysList.count() - 1, 0));
}
