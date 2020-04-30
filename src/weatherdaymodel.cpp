#include "weatherdaymodel.h"

/* ~~~ WeatherDay ~~~ */

WeatherDay::WeatherDay() {}

/* ~~~ WeatherHourListModel ~~~ */

WeatherDayListModel::WeatherDayListModel(WeatherLocation* location)
{
    connect(location, &WeatherLocation::weatherRefresh, this, &WeatherDayListModel::refreshDaysFromForecasts);
}

int WeatherDayListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return daysList.size();
}

QVariant WeatherDayListModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
}

void WeatherDayListModel::refreshDaysFromForecasts() {}

void WeatherDayListModel::updateUi() {}
