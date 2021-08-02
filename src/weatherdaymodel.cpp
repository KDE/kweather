/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
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
    return m_weatherDays.size();
}

QVariant WeatherDayListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= static_cast<int>(m_weatherDays.size()) || index.row() < 0) {
        return {};
    }
    if (role == Roles::DayItemRole) {
        return QVariant::fromValue(m_weatherDays.at(index.row()));
    }
    return {};
}

QHash<int, QByteArray> WeatherDayListModel::roleNames() const
{
    return {{Roles::DayItemRole, "dayItem"}};
}

WeatherDay *WeatherDayListModel::get(int index)
{
    WeatherDay *ret = nullptr;
    if (index < 0 || index >= static_cast<int>(m_forecasts.dailyWeatherForecast().size()))
        return ret;

    ret = m_weatherDays.at(index);
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::CppOwnership); // prevent segfaults from js garbage collecting
    return ret;
}

void WeatherDayListModel::refreshDaysFromForecasts(KWeatherCore::WeatherForecast forecasts)
{
    if (forecasts.dailyWeatherForecast().empty())
        return;

    m_forecasts = forecasts;

    // skip outdated data
    int skipped = 0;
    auto date = QDate::currentDate();
    while (forecasts.dailyWeatherForecast().at(skipped).date() < date)
        skipped++;

    if (forecasts.dailyWeatherForecast().size() - skipped > m_weatherDays.size()) {
        beginInsertRows(QModelIndex(), m_weatherDays.size(), forecasts.dailyWeatherForecast().size() - 1 - skipped);
        for (auto i = m_weatherDays.size(); i < forecasts.dailyWeatherForecast().size() - skipped; i++) {
            auto weatherDay = new WeatherDay(forecasts, i, this);
            QQmlEngine::setObjectOwnership(weatherDay, QQmlEngine::CppOwnership); // prevent segfaults from js garbage collecting
            m_weatherDays.push_back(weatherDay);
        }
        endInsertRows();
    } else if (forecasts.dailyWeatherForecast().size() - skipped < m_weatherDays.size()) {
        beginRemoveRows(QModelIndex(), forecasts.dailyWeatherForecast().size() - skipped, m_weatherDays.size() - 1);
        for (auto i = m_weatherDays.size() - forecasts.dailyWeatherForecast().size() + skipped; i > 0; i--) {
            m_weatherDays.back()->deleteLater();
            m_weatherDays.pop_back();
        }
        endRemoveRows();
    }

    Q_EMIT weatherRefresh(forecasts);
}

void WeatherDayListModel::updateUi()
{
    Q_EMIT dataChanged(createIndex(0, 0), createIndex(m_forecasts.dailyWeatherForecast().size() - 1, 0));
}
