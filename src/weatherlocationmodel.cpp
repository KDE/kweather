#include "weatherlocationmodel.h"
#include "weatherdaymodel.h"

/* ~~~ WeatherLocation ~~~ */
WeatherLocation::WeatherLocation() {
    this->weatherDayListModel_ = new WeatherDayListModel(this);
    this->weatherHourListModel_ = new WeatherHourListModel(this);
}

void WeatherLocation::updateData(QList<AbstractWeatherForecast*> fc)
{
    forecasts_.clear(); // don't need to delete pointers, they were already deleted by api class
    forecasts_ = fc; // just assign new list
    emit weatherRefresh(fc);
}

WeatherLocation::WeatherLocation(AbstractWeatherAPI* weatherBackendProvider, QString locationName, float latitude,
                                 float longitude)
{
    this->weatherBackendProvider_ = weatherBackendProvider;
    this->locationName_ = locationName;
    this->latitude_ = latitude;
    this->longitude_ = longitude;
    this->weatherDayListModel_ = new WeatherDayListModel(this);
    this->weatherHourListModel_ = new WeatherHourListModel(this);

    connect(this->weatherBackendProvider(), &AbstractWeatherAPI::updated, this, &WeatherLocation::updateData,
            Qt::UniqueConnection);
}

WeatherLocation::WeatherLocation(AbstractWeatherAPI* weatherBackendProvider, QString locationName, float latitude,
                                 float longitude, QList<AbstractWeatherForecast*> forecasts)
{
    this->weatherBackendProvider_ = weatherBackendProvider;
    this->locationName_ = locationName;
    this->latitude_ = latitude;
    this->longitude_ = longitude;
    this->forecasts_ = forecasts;
    this->weatherDayListModel_ = new WeatherDayListModel(this);
    this->weatherHourListModel_ = new WeatherHourListModel(this);

    connect(this->weatherBackendProvider(), &AbstractWeatherAPI::updated, this, &WeatherLocation::updateData,
            Qt::UniqueConnection);
}

/* ~~~ WeatherLocationListModel ~~~ */
WeatherLocationListModel::WeatherLocationListModel(QObject* parent) {}

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

void WeatherLocationListModel::insert(int index, WeatherLocation* weatherLocation)
{
    if ((index < 0) || (index > locationsList.count()))
        return;

    emit beginInsertRows(QModelIndex(), index, index);
    locationsList.insert(index, weatherLocation);
    emit endInsertRows();
}

void WeatherLocationListModel::remove(int index)
{
    if ((index < 0) || (index >= locationsList.count()))
        return;

    emit beginRemoveRows(QModelIndex(), index, index);
    locationsList.removeAt(index);
    emit endRemoveRows();
}

WeatherLocation* WeatherLocationListModel::get(int index)
{
    if ((index < 0) || (index >= locationsList.count()))
        return new WeatherLocation(); // TODO risk of nullptr fields being deref

    return locationsList.at(index);
}
