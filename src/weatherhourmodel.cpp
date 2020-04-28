#include "weatherhourmodel.h"

/* ~~~ WeatherHour ~~~ */

WeatherHour::WeatherHour()
{
}

/* ~~~ WeatherHourListModel ~~~ */

WeatherHourListModel::WeatherHourListModel(WeatherLocation* location)
{
    connect(location, &WeatherLocation::weatherRefresh, this, &WeatherHourListModel::refreshHoursFromForecasts);
}


int WeatherHourListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return hoursList.size();
}

QVariant WeatherHourListModel::data(const QModelIndex& index, int role) const
{
    
}

QHash<int, QByteArray> WeatherHourListModel::roleNames() const
{
    
}

void WeatherHourListModel::refreshHoursFromForecasts()
{
    
}

void WeatherHourListModel::updateUi()
{
    
}
