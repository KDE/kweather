#include "weatherlocationmodel.h"
#include "abstractweatherforecast.h"
#include "locationquerymodel.h"
#include "nmiweatherapi.h"
#include "weatherdaymodel.h"
#include "weatherhourmodel.h"
#include <QQmlEngine>
/* ~~~ WeatherLocation ~~~ */
WeatherLocation::WeatherLocation()
{
    this->weatherDayListModel_ = new WeatherDayListModel(this);
    this->weatherHourListModel_ = new WeatherHourListModel(this);
    this->lastUpdated_ = QDateTime::currentDateTime();
    QQmlEngine::setObjectOwnership(this->weatherDayListModel_, QQmlEngine::CppOwnership); // prevent segfaults from js garbage collecting
    QQmlEngine::setObjectOwnership(this->weatherHourListModel_, QQmlEngine::CppOwnership);
}

WeatherLocation::WeatherLocation(NMIWeatherAPI *weatherBackendProvider, QString locationName, float latitude, float longitude)
{
    this->weatherBackendProvider_ = weatherBackendProvider;
    this->locationName_ = locationName;
    this->latitude_ = latitude;
    this->longitude_ = longitude;
    this->weatherDayListModel_ = new WeatherDayListModel(this);
    this->weatherHourListModel_ = new WeatherHourListModel(this);
    this->lastUpdated_ = QDateTime::currentDateTime();
    QQmlEngine::setObjectOwnership(this->weatherDayListModel_, QQmlEngine::CppOwnership); // prevent segfaults from js garbage collecting
    QQmlEngine::setObjectOwnership(this->weatherHourListModel_, QQmlEngine::CppOwnership);
    determineCurrentForecast();

    connect(this->weatherBackendProvider(), &AbstractWeatherAPI::updated, this, &WeatherLocation::updateData, Qt::UniqueConnection);
}

WeatherLocation::WeatherLocation(NMIWeatherAPI *weatherBackendProvider, QString locationName, float latitude, float longitude, QList<AbstractWeatherForecast *> forecasts)
{
    this->weatherBackendProvider_ = weatherBackendProvider;
    this->locationName_ = locationName;
    this->latitude_ = latitude;
    this->longitude_ = longitude;
    this->forecasts_ = forecasts;
    this->weatherDayListModel_ = new WeatherDayListModel(this);
    this->weatherHourListModel_ = new WeatherHourListModel(this);
    this->lastUpdated_ = QDateTime::currentDateTime();
    QQmlEngine::setObjectOwnership(this->weatherDayListModel_, QQmlEngine::CppOwnership); // prevent segfaults from js garbage collecting
    QQmlEngine::setObjectOwnership(this->weatherHourListModel_, QQmlEngine::CppOwnership);
    determineCurrentForecast();

    connect(this->weatherBackendProvider(), &AbstractWeatherAPI::updated, this, &WeatherLocation::updateData, Qt::UniqueConnection);
}

void WeatherLocation::updateData(QList<AbstractWeatherForecast *> fc)
{
    forecasts_.clear(); // don't need to delete pointers, they were already deleted by api class
    forecasts_ = fc;    // just assign new list
    determineCurrentForecast();
    this->lastUpdated_ = QDateTime::currentDateTime();

    emit weatherRefresh(fc);
}

void WeatherLocation::determineCurrentForecast()
{
    if (forecasts().count() == 0) {
        currentForecast_ = new AbstractWeatherForecast(locationName_, "", "Unknown", "weather-none-available", "weather-none-available", QDateTime::currentDateTime(), latitude(), longitude(), 0, 0, 0, 0, 0, 0, 0, 0);
    } else {
        long long minSecs = -1;
        QDateTime current = QDateTime::currentDateTime();

        // get closest forecast to current time
        for (auto forecast : forecasts()) {
            if (minSecs == -1 || minSecs > forecast->time().secsTo(current)) {
                currentForecast_ = forecast;
                minSecs = forecast->time().secsTo(current);
            }
        }
    }
    QQmlEngine::setObjectOwnership(currentForecast_, QQmlEngine::CppOwnership); // prevent segfaults from js garbage collecting
    emit currentForecastChange();
}

/* ~~~ WeatherLocationListModel ~~~ */
WeatherLocationListModel::WeatherLocationListModel(QObject *parent)
{
}

int WeatherLocationListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return locationsList.size();
}

QVariant WeatherLocationListModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

void WeatherLocationListModel::updateUi()
{
    emit dataChanged(createIndex(0, 0), createIndex(locationsList.count() - 1, 0));
}

void WeatherLocationListModel::insert(int index, WeatherLocation *weatherLocation)
{
    if ((index < 0) || (index > locationsList.count()))
        return;

    QQmlEngine::setObjectOwnership(weatherLocation, QQmlEngine::CppOwnership);
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

WeatherLocation *WeatherLocationListModel::get(int index)
{
    if ((index < 0) || (index >= locationsList.count()))
        return {};

    return locationsList.at(index);
}

void WeatherLocationListModel::move(int oldIndex, int newIndex)
{
    if (oldIndex < 0 || oldIndex >= locationsList.count() || newIndex < 0 || newIndex >= locationsList.count())
        return;
    locationsList.move(oldIndex, newIndex);
}

void WeatherLocationListModel::addLocation(LocationQueryResult *ret)
{
    qDebug() << "add location";
    auto api = new NMIWeatherAPI();
    api->setLocation(ret->latitude(), ret->latitude());
    auto location = new WeatherLocation(api, ret->name(), ret->latitude(), ret->longitude());
    api->update();
    insert(this->locationsList.count(), location);
}
