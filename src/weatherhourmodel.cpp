#include "weatherhourmodel.h"

/* ~~~ WeatherHour ~~~ */

WeatherHour::WeatherHour() {}

WeatherHour::WeatherHour(AbstractWeatherForecast* forecast)
{
    
}

/* ~~~ WeatherHourListModel ~~~ */

WeatherHourListModel::WeatherHourListModel(WeatherLocation* location)
{
    connect(location, &WeatherLocation::weatherRefresh, this, &WeatherHourListModel::refreshHoursFromForecasts);
}

int WeatherHourListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return hoursList.size();
}

QVariant WeatherHourListModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
}

void WeatherHourListModel::refreshHoursFromForecasts(QList<AbstractWeatherForecast*> forecasts) 
{
    // clear forecasts
    emit beginRemoveRows(QModelIndex(), 0, hoursList.count()-1);
    hoursList.clear();
    emit endRemoveRows();
    
    emit beginInsertRows(QModelIndex(), 0, forecasts.count() - 1);
    
    for (auto forecast : forecasts) {
        hoursList.append(new WeatherHour(forecast)); // TODO
    }
    
    emit endInsertRows();
}

void WeatherHourListModel::updateUi() {}
