/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
#include "weatherhourmodel.h"
#include "weatherlocation.h"
#include <QQmlEngine>
WeatherHourListModel::WeatherHourListModel(WeatherLocation *location)
    : QAbstractListModel(location)
{
    connect(location, &WeatherLocation::weatherRefresh, this, &WeatherHourListModel::refreshHoursFromForecasts);
}

int WeatherHourListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(m_currentDay >= static_cast<int>(m_hours.size()) || m_currentDay < 0)
        return 0;

    return m_hours.at(m_currentDay).size();
}

QVariant WeatherHourListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || m_currentDay >= static_cast<int>(m_hours.size()) || index.row() >= static_cast<int>(m_hours.at(m_currentDay).size())) {
        return {};
    }
    if (role == Roles::HourItemRole) {
        return QVariant::fromValue(m_hours.at(m_currentDay).at(index.row()));
    }
    return {};
}

QHash<int, QByteArray> WeatherHourListModel::roleNames() const
{
    return {{Roles::HourItemRole, "hourItem"}};
}

WeatherHour *WeatherHourListModel::get(int index)
{
    WeatherHour *ret;
    if (index < 0 || m_currentDay >= static_cast<int>(m_hours.size()) || index >= static_cast<int>(m_hours.at(m_currentDay).size())) {
        return nullptr;
    } else {
        ret = m_hours.at(m_currentDay).at(index);
    }
    // it's kind of dumb how much seems to be garbage collected by js...
    // this fixes segfaults with scrolling with the hour view
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::CppOwnership);
    return ret;
}

void WeatherHourListModel::refreshHoursFromForecasts(QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> forecast)
{
    Q_EMIT layoutAboutToBeChanged();

    // sync days
    if(forecast->dailyWeatherForecast().size() > m_hours.size())
    {
        auto i = m_hours.size();
        for(; i < forecast->dailyWeatherForecast().size(); i++)
        {
            m_hours.push_back({});
            const auto &hours = forecast->dailyWeatherForecast().at(i).hourlyWeatherForecast();
            m_hours.back().reserve(hours.size());

            for(auto j = 0; j < static_cast<int>(hours.size()); j++)
            {
                m_hours.back().push_back(new WeatherHour(forecast, i, j, this));
            }
        }
    } else if (forecast->dailyWeatherForecast().size() < m_hours.size())
    {
        auto i = m_hours.size() - forecast->dailyWeatherForecast().size();
        for(; i > 0; i--)
        {
            auto &hours = m_hours.back();
            for(auto it = hours.begin(); it != hours.end(); it++)
                (*it)->deleteLater();

            m_hours.pop_back();
        }
    }

    // sync hours

    const auto &days = forecast->dailyWeatherForecast();
    int dayIndex = 0;
    for(const auto &day : days)
    {
        const auto &newHours = day.hourlyWeatherForecast();
        auto &oldHours = m_hours.at(dayIndex++);
        if(newHours.size() > oldHours.size())
        {
            auto hourIndex = oldHours.size();
            for(; hourIndex < newHours.size(); hourIndex++)
            {
                auto *weatherHour = new WeatherHour(forecast, dayIndex, hourIndex, this);
                QQmlEngine::setObjectOwnership(weatherHour, QQmlEngine::CppOwnership); // prevent segfaults from js garbage collecting
                oldHours.push_back(weatherHour);
            }
        } else if (newHours.size() < oldHours.size())
        {
            auto j = oldHours.size() - newHours.size();
            while(j--)
            {
                oldHours.back()->deleteLater();
                oldHours.pop_back();
            }
        }
    }

    m_currentDay = 0;

    Q_EMIT weatherRefresh(forecast);
    Q_EMIT layoutChanged();
}

void WeatherHourListModel::updateHourView(int index)
{
    Q_EMIT layoutAboutToBeChanged();
    m_currentDay = index;
    Q_EMIT layoutChanged();
}

void WeatherHourListModel::updateUi()
{
    auto &day = m_hours.at(m_currentDay);
    for (auto h : day) {
        Q_EMIT h->propertyChanged();
    }
    Q_EMIT dataChanged(createIndex(0, 0), createIndex(m_hours.at(m_currentDay).size() - 1, 0));
}

WeatherHour *WeatherHourListModel::currentForecast() const
{
    if(m_hours.empty() || m_hours.begin()->empty())
        return nullptr;

    return m_hours.begin()->front();
}
