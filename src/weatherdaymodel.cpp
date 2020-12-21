/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "weatherdaymodel.h"
#include "weatherlocation.h"
#include <QQmlEngine>
WeatherDayListModel::WeatherDayListModel(WeatherLocation *location)
    : QAbstractListModel(location)
{
    connect(location, &WeatherLocation::weatherRefresh, this, &WeatherDayListModel::refreshDaysFromForecasts);
}

int WeatherDayListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return daysVec.size();
}

QVariant WeatherDayListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= static_cast<int>(daysVec.size()) || index.row() < 0) {
        return {};
    }
    if (role == Roles::DayItemRole) {
        return QVariant::fromValue(daysVec.at(index.row()));
    }
    return {};
}

QHash<int, QByteArray> WeatherDayListModel::roleNames() const
{
    return {{Roles::DayItemRole, "dayItem"}};
}

WeatherDay *WeatherDayListModel::get(int index)
{
    if (index < 0 || index >= static_cast<int>(daysVec.size()))
        return {};
    return daysVec.at(index);
}

void WeatherDayListModel::refreshDaysFromForecasts(QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> forecasts)
{
    sunriseVec.clear();
    Q_EMIT layoutAboutToBeChanged();
    Q_EMIT beginRemoveRows(QModelIndex(), 0, daysVec.size() - 1);
    auto oldList = daysVec;
    daysVec.clear();
    Q_EMIT endRemoveRows();

    Q_EMIT beginInsertRows(QModelIndex(), 0, forecasts->dailyWeatherForecast().size() - 1);

    // add weatherdays with forecast day lists
    for (auto forecast : forecasts->dailyWeatherForecast()) {
        sunriseVec.push_back(forecast.sunrise());
        WeatherDay *weatherDay = new WeatherDay(forecast);
        QQmlEngine::setObjectOwnership(weatherDay, QQmlEngine::CppOwnership); // prevent segfaults from js garbage collecting
        daysVec.push_back(weatherDay);
    }

    Q_EMIT endInsertRows();
    Q_EMIT layoutChanged();
    for (auto ptr : oldList)
        ptr->deleteLater();
}

void WeatherDayListModel::updateUi()
{
    for (auto h : daysVec) {
        Q_EMIT h->propertyChanged();
    }
    Q_EMIT dataChanged(createIndex(0, 0), createIndex(daysVec.size() - 1, 0));
}
const std::vector<KWeatherCore::Sunrise> &WeatherDayListModel::sunrise() const
{
    return sunriseVec;
}
const std::vector<WeatherDay *> &WeatherDayListModel::days() const
{
    return daysVec;
}
