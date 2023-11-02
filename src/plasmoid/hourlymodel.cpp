/*
    SPDX-FileCopyrightText: 2021 HanYoung <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/
#include "hourlymodel.h"

#include <QProcess>

QVariant HourlyModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount(QModelIndex()))
        return {};

    switch (role) {
    case Time:
        return QLocale().toString(getNthHour(index.row()).date(), QLocale::ShortFormat).toLower();
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
    if (m_location.dailyWeatherForecast().empty()) {
        return 0;
    }
    return 24;
}
QHash<int, QByteArray> HourlyModel::roleNames() const
{
    return {{Time, "time"}, {Icon, "weatherIcon"}, {Description, "description"}, {Temperature, "temperature"}, {Precipitation, "precipitation"}};
}
void HourlyModel::loadForecast(KWeatherCore::WeatherForecast forecast)
{
    beginResetModel();
    m_location = std::move(forecast);
    endResetModel();
    Q_EMIT reseted();
}
const KWeatherCore::HourlyWeatherForecast &HourlyModel::getNthHour(int index) const
{
    auto dayIndex{0};
    auto hourIndex{0};
    for (const auto &day : m_location.dailyWeatherForecast()) {
        if ((int)day.hourlyWeatherForecast().size() - 1 < index) {
            index -= day.hourlyWeatherForecast().size();
            dayIndex++;
        } else {
            hourIndex = index;
            break;
        }
    }

    return m_location.dailyWeatherForecast().at(dayIndex).hourlyWeatherForecast().at(hourIndex);
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
void HourlyModel::openKWeather()
{
    auto m_process = new QProcess(this);
    m_process->start(QStringLiteral("kweather"), QStringList());
}

#include "moc_hourlymodel.cpp"
