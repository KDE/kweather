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
    return m_forecasts->dailyWeatherForecast().size();
}

QVariant WeatherDayListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= static_cast<int>(m_forecasts->dailyWeatherForecast().size()) || index.row() < 0) {
        return {};
    }
    if (role == Roles::DayItemRole) {
        return QVariant::fromValue(WeatherDay(m_forecasts->dailyWeatherForecast().at(index.row())));
    }
    return {};
}

QHash<int, QByteArray> WeatherDayListModel::roleNames() const
{
    return {{Roles::DayItemRole, "dayItem"}};
}

WeatherDay WeatherDayListModel::get(int index)
{
    if (index < 0 || index >= static_cast<int>(m_forecasts->dailyWeatherForecast().size()))
        return {};
    return WeatherDay(m_forecasts->dailyWeatherForecast().at(index));
}

void WeatherDayListModel::refreshDaysFromForecasts(QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> forecasts)
{
    Q_EMIT layoutAboutToBeChanged();
    m_forecasts = std::move(forecasts);
    Q_EMIT layoutChanged();
}

void WeatherDayListModel::updateUi()
{
    Q_EMIT dataChanged(createIndex(0, 0), createIndex(m_forecasts->dailyWeatherForecast().size() - 1, 0));
}
