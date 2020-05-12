/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "weatherlocationmodel.h"
#include "abstractweatherapi.h"
#include "abstractweatherforecast.h"
#include "geoiplookup.h"
#include "locationquerymodel.h"
#include "nmiweatherapi2.h"
#include "weatherdaymodel.h"
#include "weatherhourmodel.h"
#include <KConfigCore/KConfigGroup>
#include <KConfigCore/KSharedConfig>
#include <QDir>
#include <QFile>
#include <QQmlEngine>
#include <QtCore/QJsonArray>
#include <stdlib.h>

const QString WEATHER_LOCATIONS_CFG_GROUP = "WeatherLocations";
const QString WEATHER_LOCATIONS_CFG_KEY = "locationsList";

/* ~~~ WeatherLocation ~~~ */
WeatherLocation::WeatherLocation(AbstractWeatherForecast *forecast)
{
    this->weatherDayListModel_ = new WeatherDayListModel(this);
    this->weatherHourListModel_ = new WeatherHourListModel(this);
    this->lastUpdated_ = QDateTime::currentDateTime();
    this->forecast_ = forecast;
    QQmlEngine::setObjectOwnership(this->weatherDayListModel_, QQmlEngine::CppOwnership); // prevent segfaults from js garbage collecting
    QQmlEngine::setObjectOwnership(this->weatherHourListModel_, QQmlEngine::CppOwnership);
}

WeatherLocation::WeatherLocation(AbstractWeatherAPI *weatherBackendProvider, QString locationId, QString locationName, float latitude, float longitude, AbstractWeatherForecast *forecast)
{
    this->weatherBackendProvider_ = weatherBackendProvider;
    this->locationId_ = locationId;
    this->locationName_ = locationName;
    this->latitude_ = latitude;
    this->longitude_ = longitude;
    this->forecast_ = forecast;
    this->weatherDayListModel_ = new WeatherDayListModel(this);
    this->weatherHourListModel_ = new WeatherHourListModel(this);
    this->lastUpdated_ = forecast == nullptr ? QDateTime::currentDateTime() : forecast->timeCreated();
    QQmlEngine::setObjectOwnership(this->weatherDayListModel_, QQmlEngine::CppOwnership); // prevent segfaults from js garbage collecting
    QQmlEngine::setObjectOwnership(this->weatherHourListModel_, QQmlEngine::CppOwnership);
    determineCurrentForecast();

    if (weatherBackendProvider != nullptr)
        weatherBackendProvider->setLocation(latitude, longitude);

    connect(this->weatherBackendProvider(), &AbstractWeatherAPI::updated, this, &WeatherLocation::updateData, Qt::UniqueConnection);
}

WeatherLocation *WeatherLocation::fromJson(const QJsonObject &obj)
{
    auto api = new NMIWeatherAPI2(obj["locationId"].toString());
    api->setTimeZone(obj["timezone"].toString());
    return new WeatherLocation(api, obj["locationId"].toString(), obj["locationName"].toString(), obj["latitude"].toDouble(), obj["longitude"].toDouble());
}

QJsonObject WeatherLocation::toJson()
{
    QJsonObject obj;
    obj["locationId"] = locationId();
    obj["locationName"] = locationName();
    obj["latitude"] = latitude();
    obj["longitude"] = longitude();
    obj["timezone"] = weatherBackendProvider_->getTimeZone();
    return obj;
}

void WeatherLocation::updateData(AbstractWeatherForecast *fc)
{
    // only update if the forecast is newer
    if (forecast_ != nullptr && fc->timeCreated().toSecsSinceEpoch() < forecast_->timeCreated().toSecsSinceEpoch())
        return;

    forecast_ = fc; // don't need to delete pointers, they were already deleted by api class
    determineCurrentForecast();
    this->lastUpdated_ = fc->timeCreated();
    emit weatherRefresh(fc);
    emit stopLoadingIndicator();
    writeToCache(fc);

    emit propertyChanged();
}

void WeatherLocation::determineCurrentForecast()
{
    if (forecast() == nullptr || forecast()->hourlyForecasts().count() == 0) {
        currentWeather_ = new AbstractHourlyWeatherForecast(QDateTime::currentDateTime(), "Unknown", "weather-none-available", "weather-none-available", 0, 0, AbstractHourlyWeatherForecast::WindDirection::N, 0, 0, 0, 0, 0);
    } else {
        long long minSecs = -1;
        QDateTime current = QDateTime::currentDateTime();

        // get closest forecast to current time
        for (auto forecast : forecast()->hourlyForecasts()) {
            if (minSecs == -1 || minSecs > llabs(forecast->date().secsTo(current))) {
                currentWeather_ = forecast;
                minSecs = llabs(forecast->date().secsTo(current));
            }
        }
    }
    QQmlEngine::setObjectOwnership(currentWeather_, QQmlEngine::CppOwnership); // prevent segfaults from js garbage collecting
    emit currentForecastChange();
}

void WeatherLocation::writeToCache(AbstractWeatherForecast *fc)
{
    QFile file;
    QString url;
    url = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir dir(url.append(QString("/cache/%1/%2/").arg(QString::number(static_cast<int>(fc->latitude() * 100))).arg(QString::number(static_cast<int>(fc->longitude() * 100))))); // create cache location
    if (!dir.exists())
        dir.mkpath(".");
    // should be this path: /home/user/.cache/kweather/cache/7000/3000 for location with coordinate 70.00 30.00
    file.setFileName(dir.path() + "/cache.json");
    file.open(QIODevice::WriteOnly);
    file.write(convertToJson(fc).toJson(QJsonDocument::Compact)); // write json
    file.close();
}
QJsonDocument WeatherLocation::convertToJson(AbstractWeatherForecast *fc)
{
    QJsonDocument doc;
    doc.setObject(fc->toJson());
    return doc;
}

/* ~~~ WeatherLocationListModel ~~~ */
WeatherLocationListModel::WeatherLocationListModel(QObject *parent)
{
    load();
    if (locationsList.count() == 0) { // no location
        geoPtr = new GeoIPLookup();
        connect(geoPtr, &GeoIPLookup::finished, this, &WeatherLocationListModel::getDefaultLocation);
    }
}

void WeatherLocationListModel::load()
{
    // load locations from kconfig
    auto config = KSharedConfig::openConfig();
    KConfigGroup group = config->group(WEATHER_LOCATIONS_CFG_GROUP);
    QJsonDocument doc = QJsonDocument::fromJson(group.readEntry(WEATHER_LOCATIONS_CFG_KEY, "{}").toUtf8());
    for (QJsonValueRef r : doc.array()) {
        QJsonObject obj = r.toObject();
        locationsList.append(WeatherLocation::fromJson(obj));
    }
}

void WeatherLocationListModel::save()
{
    QJsonArray arr;
    for (auto lc : locationsList) {
        arr.push_back(lc->toJson());
    }
    QJsonObject obj;
    obj["list"] = arr;

    auto config = KSharedConfig::openConfig();
    KConfigGroup group = config->group(WEATHER_LOCATIONS_CFG_GROUP);
    group.writeEntry(WEATHER_LOCATIONS_CFG_KEY, QString(QJsonDocument(arr).toJson(QJsonDocument::Compact)));
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
    for (auto l : locationsList) {
        emit l->propertyChanged();
        l->weatherDayListModel()->updateUi();
        l->weatherHourListModel()->updateUi();
        if (l->forecast() != nullptr) {
            for (auto h : l->forecast()->hourlyForecasts()) {
                emit h->propertyChanged();
            }
        }
    }
}

void WeatherLocationListModel::insert(int index, WeatherLocation *weatherLocation)
{
    if ((index < 0) || (index > locationsList.count()))
        return;

    QQmlEngine::setObjectOwnership(weatherLocation, QQmlEngine::CppOwnership);
    emit beginInsertRows(QModelIndex(), index, index);
    locationsList.insert(index, weatherLocation);
    emit endInsertRows();

    save();
}

void WeatherLocationListModel::remove(int index)
{
    if ((index < 0) || (index >= locationsList.count()))
        return;

    emit beginRemoveRows(QModelIndex(), index, index);
    locationsList.removeAt(index);
    emit endRemoveRows();

    save();
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
    auto api = new NMIWeatherAPI2(ret->geonameId());
    qDebug() << "lat" << ret->latitude();
    qDebug() << "lgn" << ret->longitude();
    api->setLocation(ret->latitude(), ret->longitude());
    auto location = new WeatherLocation(api, ret->geonameId(), ret->name(), ret->latitude(), ret->longitude());
    api->update();
    insert(this->locationsList.count(), location);
}

void WeatherLocationListModel::getDefaultLocation()
{
    // default location, use timestamp as id
    long id = QDateTime::currentSecsSinceEpoch();
    auto api = new NMIWeatherAPI2(QString::number(id));
    api->setTimeZone(geoPtr->timeZone());
    api->setLocation(geoPtr->latitude(), geoPtr->longitude());
    auto location = new WeatherLocation(api, QString::number(id), geoPtr->name(), geoPtr->latitude(), geoPtr->longitude());
    api->update();
    insert(0, location);
}
