#include "weatherlocationmodel.h"
#include "weatherdaymodel.h"

/* ~~~ WeatherLocation ~~~ */
WeatherLocation::WeatherLocation() {}

WeatherLocation::WeatherLocation(QString locationName,
                                 float   latitude,
                                 float   longitude)
{
  this->locationName_         = locationName;
  this->latitude_             = latitude;
  this->longitude_            = longitude;
  this->weatherDayListModel_  = new WeatherDayListModel(this);
  this->weatherHourListModel_ = new WeatherHourListModel(this);
}

WeatherLocation::WeatherLocation(QString                         locationName,
                                 float                           latitude,
                                 float                           longitude,
                                 QList<AbstractWeatherForecast *>forecasts)
{
  this->locationName_         = locationName;
  this->latitude_             = latitude;
  this->longitude_            = longitude;
  this->forecasts_            = forecasts;
  this->weatherDayListModel_  = new WeatherDayListModel(this);
  this->weatherHourListModel_ = new WeatherHourListModel(this);
}

/* ~~~ WeatherLocationListModel ~~~ */
WeatherLocationListModel::WeatherLocationListModel(QObject *parent) {}

int WeatherLocationListModel::rowCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  return locationsList.size();
}

QVariant WeatherLocationListModel::data(const QModelIndex& index, int role) const
{
  return QVariant();
}

void WeatherLocationListModel::updateUi()
{
  emit dataChanged(createIndex(0, 0), createIndex(locationsList.count() - 1, 0));
}

void WeatherLocationListModel::insert(int index, WeatherLocation *weatherLocation)
{
  if ((index < 0) || (index > locationsList.count())) return;

  emit beginInsertRows(QModelIndex(), index, index);
  locationsList.insert(index, weatherLocation);
  emit endInsertRows();
}

void WeatherLocationListModel::remove(int index)
{
  if ((index < 0) || (index >= locationsList.count())) return;

  emit beginRemoveRows(QModelIndex(), index, index);
  locationsList.removeAt(index);
  emit endRemoveRows();
}

WeatherLocation * WeatherLocationListModel::get(int index)
{
  if ((index < 0) || (index >= locationsList.count())) return new WeatherLocation(
      "",
      0,
      0);

  return locationsList.at(index);
}
