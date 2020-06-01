/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "weatherhourmodel.h"
#include <QQmlEngine>

/* ~~~ WeatherHour ~~~ */

WeatherHour::WeatherHour()
{
    this->weatherDescription_ = "Unknown";
    this->weatherIcon_ = "weather-none-available";
    this->date_ = QDateTime::currentDateTime();
    this->windDirection_ = "N";
}

WeatherHour::WeatherHour(AbstractHourlyWeatherForecast &forecast)
{
    switch (forecast.windDirection()) {
    case Kweather::WindDirection::N:
        this->windDirection_ = "N";
        break;
    case Kweather::WindDirection::NE:
        this->windDirection_ = "NE";
        break;
    case Kweather::WindDirection::E:
        this->windDirection_ = "E";
        break;
    case Kweather::WindDirection::SE:
        this->windDirection_ = "SE";
        break;
    case Kweather::WindDirection::S:
        this->windDirection_ = "S";
        break;
    case Kweather::WindDirection::SW:
        this->windDirection_ = "SW";
        break;
    case Kweather::WindDirection::W:
        this->windDirection_ = "W";
        break;
    case Kweather::WindDirection::NW:
        this->windDirection_ = "NW";
        break;
    }
    this->weatherDescription_ = forecast.weatherDescription();
    this->weatherIcon_ = forecast.weatherIcon();
    this->precipitation_ = forecast.precipitationAmount();
    this->fog_ = forecast.fog();
    this->windSpeed_ = forecast.windSpeed();
    this->temperature_ = forecast.temperature();
    this->humidity_ = forecast.humidity();
    this->pressure_ = forecast.pressure();
    this->date_ = QDateTime(forecast.date().date(), QTime(forecast.date().time().hour(), 0));
}

/* ~~~ WeatherHourListModel ~~~ */

WeatherHourListModel::WeatherHourListModel(WeatherLocation *location)
{
    connect(location, &WeatherLocation::weatherRefresh, this, &WeatherHourListModel::refreshHoursFromForecasts);
}

int WeatherHourListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (day >= dayList.count() || day < 0)
        return hoursList.count();
    if (day == dayList.count() - 1)
        return hoursList.count() - dayList.at(day);
    return dayList.at(day + 1) - dayList.at(day);
}

QVariant WeatherHourListModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

WeatherHour *WeatherHourListModel::get(int index)
{
    WeatherHour *ret;
    if (index < 0 || day >= dayList.count() || (index + dayList.at(day)) >= hoursList.count()) {
        return {};
    } else {
        ret = hoursList.at(index + dayList.at(day));
    }
    // it's kind of dumb how much seems to be garbage collected by js...
    // this fixes segfaults with scrolling with the hour view
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::CppOwnership);
    return ret;
}

void WeatherHourListModel::refreshHoursFromForecasts(AbstractWeatherForecast& forecast)
{
    // clear forecasts
    emit layoutAboutToBeChanged();
    day = 0;
    hoursList.clear();
    dayList.clear();
    // insert forecasts
    int currentDay = -1;
    int index = 0;
    for (auto hourForecast : forecast.hourlyForecasts()) {
        if (currentDay != hourForecast.date().date().day()) {
            currentDay = hourForecast.date().date().day();
            dayList.append(index);
        }
        auto *weatherHour = new WeatherHour(hourForecast);
        QQmlEngine::setObjectOwnership(weatherHour, QQmlEngine::CppOwnership); // prevent segfaults from js garbage collecting
        hoursList.append(weatherHour);

        index++;
    }

    std::sort(hoursList.begin(), hoursList.end(), [](WeatherHour *h1, WeatherHour *h2) -> bool { return h1->date() < h2->date(); });
    emit layoutChanged();
}

void WeatherHourListModel::updateHourView(int index)
{
    emit layoutAboutToBeChanged();
    day = index;
    emit layoutChanged();
}

void WeatherHourListModel::updateUi()
{
    for (auto h : hoursList) {
        emit h->propertyChanged();
    }
    emit dataChanged(createIndex(0, 0), createIndex(hoursList.count() - 1, 0));
}
