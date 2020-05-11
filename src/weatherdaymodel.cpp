#include "weatherdaymodel.h"
#include <QQmlEngine>
#include <memory>
#include <set>

/* ~~~ WeatherDay ~~~ */

WeatherDay::WeatherDay()
{
}

WeatherDay::WeatherDay(AbstractDailyWeatherForecast* dailyForecast)
{
    this->maxTemp_ = dailyForecast->maxTemp();
    this->minTemp_ = dailyForecast->minTemp();
    this->weatherIcon_ = dailyForecast->weatherIcon();
    this->weatherDescription_ = dailyForecast->weatherDescription();
    this->date_ = dailyForecast->date();
}

/* ~~~ WeatherHourListModel ~~~ */

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
    return QVariant();
}

WeatherDay *WeatherDayListModel::get(int index)
{
    if (index < 0 || index >= daysList.count())
        return {};
    return daysList.at(index);
}

void WeatherDayListModel::refreshDaysFromForecasts(AbstractWeatherForecast* forecasts)
{
    emit layoutAboutToBeChanged();
    emit beginRemoveRows(QModelIndex(), 0, daysList.count() - 1);
    daysList.clear();
    emit endRemoveRows();

    emit beginInsertRows(QModelIndex(), 0, forecasts->dailyForecasts().count() - 1);

    // add weatherdays with forecast day lists
    for (auto forecast : forecasts->dailyForecasts()) {
        WeatherDay* weatherDay = new WeatherDay(forecast);
        QQmlEngine::setObjectOwnership(weatherDay, QQmlEngine::CppOwnership); // prevent segfaults from js garbage collecting
        daysList.append(weatherDay);
    }
    std::sort(daysList.begin(), daysList.end(), [](WeatherDay *h1, WeatherDay *h2) -> bool { return h1->date() < h2->date(); });

    emit endInsertRows();
    emit layoutChanged();
}

void WeatherDayListModel::updateUi()
{
    for (auto h : daysList) {
        emit h->propertyChanged();
    }
    emit dataChanged(createIndex(0, 0), createIndex(daysList.count() - 1, 0));
}
