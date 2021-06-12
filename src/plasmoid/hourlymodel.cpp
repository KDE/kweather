/*
    SPDX-FileCopyrightText: 2021 HanYoung <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/
#include "hourlymodel.h"
#include "locationmodel.h"
QVariant HourlyModel::data(const QModelIndex &index, int role) const
{
    if (!m_location || index.row() < 0 || index.row() >= rowCount(QModelIndex()))
        return {};

    switch (role) {
    case Time:
        return getNthHour(index.row()).date().toString(QStringLiteral("hh ap"));
    case Icon:
        return getNthHour(index.row()).weatherIcon();
    case Description:
        return getNthHour(index.row()).weatherDescription();
    case Temperature:
        return getNthHour(index.row()).temperature();
    case Precipitation:
        return QString::number(getNthHour(index.row()).precipitationAmount());
    default:
        return {};
    }
}
int HourlyModel::rowCount(const QModelIndex &index) const
{
    Q_UNUSED(index)
    if (!m_location)
        return 0;
    else
        return 24;
}
QHash<int, QByteArray> HourlyModel::roleNames() const
{
    return {{Time, "time"}, {Icon, "weatherIcon"}, {Description, "description"},
        {Temperature, "temperature"}, {Precipitation, "precipitation"}};
}
void HourlyModel::loadForecast(QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> forecast, int index)
{
    beginResetModel();
    m_location = forecast;
    m_index = index;
    endResetModel();
    Q_EMIT reseted();
}
const KWeatherCore::HourlyWeatherForecast &HourlyModel::getNthHour(int index) const
{
    auto dayIndex {0};
    auto hourIndex {0};
    for (const auto &day : m_location->dailyWeatherForecast()) {
        if ((int)day.hourlyWeatherForecast().size() - 1 < index) {
            index -= day.hourlyWeatherForecast().size();
            dayIndex++;
        } else {
            hourIndex = index;
            break;
        }
    }

    return m_location->dailyWeatherForecast().at(dayIndex).hourlyWeatherForecast().at(hourIndex);
}

const QString &HourlyModel::location() const
{
    return m_locationModel->getLocation(m_index);
}
const QString &HourlyModel::currentIcon() const
{
    return getNthHour(0).weatherIcon();
}
const QString &HourlyModel::currentDescription() const
{
    return getNthHour(0).weatherDescription();
}
QString HourlyModel::currentTemperature() const
{
    return QString::number(getNthHour(0).temperature());
}
