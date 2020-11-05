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
    if (day >= dayVec.size() || day < 0)
        return hoursVec.size();
    if (day == dayVec.size() - 1)
        return hoursVec.size() - dayVec.at(day);
    return dayVec.at(day + 1) - dayVec.at(day);
}

QVariant WeatherHourListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || day >= dayVec.size() || (index.row() + dayVec.at(day)) >= hoursVec.size()) {
        return {};
    }
    if (role == Roles::HourItemRole) {
        return QVariant::fromValue(hoursVec.at(index.row() + dayVec.at(day)));
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
    if (index < 0 || day >= dayVec.size() || (index + dayVec.at(day)) >= hoursVec.size()) {
        return {};
    } else {
        ret = hoursVec.at(index + dayVec.at(day));
    }
    // it's kind of dumb how much seems to be garbage collected by js...
    // this fixes segfaults with scrolling with the hour view
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::CppOwnership);
    return ret;
}

void WeatherHourListModel::refreshHoursFromForecasts(QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> forecast)
{
    // clear forecasts
    Q_EMIT layoutAboutToBeChanged();
    // prevent memory leak
    for (auto ptr : hoursVec)
        ptr->deleteLater();

    day = 0;
    hoursVec.clear();
    dayVec.clear();
    // insert forecasts
    for (auto day : forecast->dailyWeatherForecast()) {
        dayVec.append(day.hourlyWeatherForecast().size());
        for (auto hour : day.hourlyWeatherForecast()) {
            auto *weatherHour = new WeatherHour(hour);
            QQmlEngine::setObjectOwnership(weatherHour, QQmlEngine::CppOwnership); // prevent segfaults from js garbage collecting
            hoursVec.append(weatherHour);
        }
    }
    Q_EMIT layoutChanged();
}

void WeatherHourListModel::updateHourView(int index)
{
    Q_EMIT layoutAboutToBeChanged();
    day = index;
    Q_EMIT layoutChanged();
}

void WeatherHourListModel::updateUi()
{
    for (auto h : hoursVec) {
        Q_EMIT h->propertyChanged();
    }
    Q_EMIT dataChanged(createIndex(0, 0), createIndex(hoursVec.size() - 1, 0));
}

WeatherHour *WeatherHourListModel::currentForecast() const
{
    return hoursVec.first();
}
