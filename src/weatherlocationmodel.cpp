/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "weatherlocationmodel.h"
#include "geoiplookup.h"
#include "geotimezone.h"
#include "locationquerymodel.h"
#include "owmweatherapi.h"
#include "weatherdaymodel.h"
#include "weatherlocation.h"

#include <KConfigCore/KConfigGroup>
#include <KConfigCore/KSharedConfig>

#include <QJsonArray>

const QString WEATHER_LOCATIONS_CFG_GROUP = QStringLiteral("WeatherLocations");
const QString WEATHER_LOCATIONS_CFG_KEY = QStringLiteral("locationsList");

/* ~~~ WeatherLocationListModel ~~~ */
WeatherLocationListModel::WeatherLocationListModel(QObject *parent)
{
    load();
}

void WeatherLocationListModel::load()
{
    // load locations from kconfig
    auto config = KSharedConfig::openConfig(QString(), KSharedConfig::FullConfig, QStandardPaths::AppConfigLocation);
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

    auto config = KSharedConfig::openConfig(QString(), KSharedConfig::FullConfig, QStandardPaths::AppConfigLocation);
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
    if (!index.isValid() || index.row() >= locationsList.count() || index.row() < 0) {
        return {};
    }
    if (role == Roles::LocationRole) {
        return QVariant::fromValue(locationsList.at(index.row()));
    }
    return {};
}

QHash<int, QByteArray> WeatherLocationListModel::roleNames() const
{
    return {{Roles::LocationRole, "location"}};
}

void WeatherLocationListModel::updateUi()
{
    emit dataChanged(createIndex(0, 0), createIndex(locationsList.count() - 1, 0));
    for (auto l : locationsList) {
        emit l->propertyChanged();
        l->weatherDayListModel()->updateUi();
        l->weatherHourListModel()->updateUi();
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
    auto location = locationsList.at(index);
    locationsList.removeAt(index);
    delete location;
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
    auto locId = ret->geonameId(), locName = ret->toponymName();
    auto lat = ret->latitude(), lon = ret->longitude();

    // obtain timezone
    auto *tz = new GeoTimeZone(ret->latitude(), ret->longitude());

    // unsuccessful timezone fetch
    connect(tz, &GeoTimeZone::networkError, this, [this] { emit networkErrorCreating(); });

    // successful timezone fetch
    connect(tz, &GeoTimeZone::finished, this, [this, locId, locName, lat, lon, tz] {
        qDebug() << "obtained timezone data";

        Kweather::Backend backendEnum;
        AbstractWeatherAPI *api;

        // create backend provider
        QSettings qsettings;
        QString backend = qsettings.value("Global/defaultBackend", Kweather::API_NMI).toString();
        if (backend == Kweather::API_NMI) {
            api = new NMIWeatherAPI2(locId, tz->getTimeZone(), lat, lon);
            backendEnum = Kweather::Backend::NMI;
        } else if (backend == Kweather::API_OWM) {
            api = new OWMWeatherAPI(locId, tz->getTimeZone(), lat, lon);
            backendEnum = Kweather::Backend::OWM;
        } else {
            api = new NMIWeatherAPI2(locId, tz->getTimeZone(), lat, lon);
            backendEnum = Kweather::Backend::NMI;
        }
        api->fetchSunriseData();

        // add location
        auto *location = new WeatherLocation(api, locId, locName, tz->getTimeZone(), lat, lon, backendEnum);
        location->update();

        insert(this->locationsList.count(), location);
    });
}

// invoked by frontend
void WeatherLocationListModel::requestCurrentLocation()
{
    geoPtr = new GeoIPLookup();
    // failure
    connect(geoPtr, &GeoIPLookup::networkError, this, [this]() { emit networkErrorCreatingDefault(); });
    // success
    connect(geoPtr, &GeoIPLookup::finished, this, &WeatherLocationListModel::addCurrentLocation);
}

void WeatherLocationListModel::addCurrentLocation()
{
    // default location, use timestamp as id
    long id = QDateTime::currentSecsSinceEpoch();

    auto api = new NMIWeatherAPI2(QString::number(id), geoPtr->timeZone(), geoPtr->latitude(), geoPtr->longitude());
    api->fetchSunriseData();
    auto location = new WeatherLocation(api, QString::number(id), geoPtr->name(), geoPtr->timeZone(), geoPtr->latitude(), geoPtr->longitude());
    location->update();

    insert(this->locationsList.count(), location);
    emit successfullyCreatedDefault();
}

void WeatherLocationListModel::changeBackend(int index, QString backend)
{
    if (index < 0 || index >= this->locationsList.count())
        return;
    if (backend == Kweather::API_OWM) {
        this->get(index)->changeBackend(Kweather::Backend::OWM);
    } else if (backend == Kweather::API_NMI) {
        this->get(index)->changeBackend(Kweather::Backend::NMI);
    } else {
        return;
    }
    this->save();
}
