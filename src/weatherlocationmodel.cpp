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
#include "geotimezone.h"
#include "global.h"
#include "locationquerymodel.h"
#include "nmisunriseapi.h"
#include "nmiweatherapi2.h"
#include "owmweatherapi.h"
#include "weatherdaymodel.h"
#include "weatherhourmodel.h"

#include <KConfigCore/KConfigGroup>
#include <KConfigCore/KSharedConfig>

#ifndef Q_OS_ANDROID
#include <QDBusConnection>
#endif

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QQmlEngine>
#include <QTimeZone>
#include <utility>

const QString WEATHER_LOCATIONS_CFG_GROUP = "WeatherLocations";
const QString WEATHER_LOCATIONS_CFG_KEY = "locationsList";

/* ~~~ WeatherLocation ~~~ */
WeatherLocation::WeatherLocation()
{
    this->weatherDayListModel_ = new WeatherDayListModel(this);
    this->weatherHourListModel_ = new WeatherHourListModel(this);
    this->lastUpdated_ = QDateTime::currentDateTime();
}

WeatherLocation::WeatherLocation(AbstractWeatherAPI *weatherBackendProvider, QString locationId, QString locationName, QString timeZone, float latitude, float longitude, Kweather::Backend backend, AbstractWeatherForecast forecast)
    : backend_(backend)
    , locationName_(std::move(locationName))
    , timeZone_(std::move(timeZone))
    , latitude_(latitude)
    , longitude_(longitude)
    , locationId_(std::move(locationId))
    , forecast_(forecast)
    , weatherBackendProvider_(weatherBackendProvider)
{
    this->weatherDayListModel_ = new WeatherDayListModel(this);
    this->weatherHourListModel_ = new WeatherHourListModel(this);
    this->lastUpdated_ = forecast.timeCreated();

    // prevent segfaults from js garbage collection
    QQmlEngine::setObjectOwnership(this->weatherDayListModel_, QQmlEngine::CppOwnership);
    QQmlEngine::setObjectOwnership(this->weatherHourListModel_, QQmlEngine::CppOwnership);

    determineCurrentForecast();

    connect(this->weatherBackendProvider(), &AbstractWeatherAPI::updated, this, &WeatherLocation::updateData, Qt::UniqueConnection);
}

WeatherLocation *WeatherLocation::fromJson(const QJsonObject &obj)
{
    AbstractWeatherAPI *api; // don't fetch sunrise information, since it will be loaded from cache
    Kweather::Backend backendEnum;
    if (obj["backend"].toInt() == 0) {
        api = new NMIWeatherAPI2(obj["locationId"].toString(), obj["timezone"].toString(), obj["latitude"].toDouble(), obj["longitude"].toDouble());
        backendEnum = Kweather::Backend::NMI;
    } else {
        api = new OWMWeatherAPI(obj["locationId"].toString(), obj["timezone"].toString(), obj["latitude"].toDouble(), obj["longitude"].toDouble());
        backendEnum = Kweather::Backend::OWM;
    }
    auto weatherLocation = new WeatherLocation(api, obj["locationId"].toString(), obj["locationName"].toString(), obj["timezone"].toString(), obj["latitude"].toDouble(), obj["longitude"].toDouble(), backendEnum, AbstractWeatherForecast());
    return weatherLocation;
}

QJsonObject WeatherLocation::toJson()
{
    QJsonObject obj;
    obj["locationId"] = locationId();
    obj["locationName"] = locationName();
    obj["latitude"] = latitude();
    obj["longitude"] = longitude();
    obj["timezone"] = timeZone_;
    obj["backend"] = static_cast<int>(backend_);
    return obj;
}

void WeatherLocation::updateData(AbstractWeatherForecast &fc)
{
    forecast_ = fc;
    determineCurrentForecast();
    lastUpdated_ = fc.timeCreated();

    emit weatherRefresh(forecast_);
    emit stopLoadingIndicator();
    writeToCache(forecast_);

    emit propertyChanged();
}

void WeatherLocation::determineCurrentForecast()
{
    delete currentWeather_;

    if (forecast().hourlyForecasts().count() == 0) {
        currentWeather_ = new WeatherHour();
    } else {
        long long minSecs = -1;
        QDateTime current = QDateTime::currentDateTime();

        // get closest forecast to current time
        for (auto forecast : forecast_.hourlyForecasts()) {
            if (minSecs == -1 || minSecs > llabs(forecast.date().secsTo(current))) {
                currentWeather_ = new WeatherHour(forecast);
                minSecs = llabs(forecast.date().secsTo(current));
            }
        }
    }
    QQmlEngine::setObjectOwnership(currentWeather_, QQmlEngine::CppOwnership); // prevent segfaults from js garbage collecting
    emit currentForecastChange();
}

void WeatherLocation::initData(AbstractWeatherForecast fc)
{
    forecast_ = fc;
    weatherBackendProvider_->setCurrentData(forecast_);
    weatherBackendProvider_->setCurrentSunriseData(fc.sunrise());
    weatherBackendProvider_->fetchSunriseData(); // TODO detect if we need to actually fetch sunrise data
    determineCurrentForecast();
    emit weatherRefresh(forecast_);
    emit propertyChanged();
}

void WeatherLocation::update()
{
    weatherBackendProvider_->update();
}

void WeatherLocation::writeToCache(AbstractWeatherForecast &fc)
{
    QFile file;
    QString url = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir dir(url.append(QString("/cache"))); // create cache location
    if (!dir.exists())
        dir.mkpath(".");
    // should be this path: /home/user/.cache/kweather/cache/1234567 for
    // location with locationID 1234567
    file.setFileName(dir.path() + "/" + this->locationId());
    file.open(QIODevice::WriteOnly);
    file.write(convertToJson(fc).toJson(QJsonDocument::Compact)); // write json
    file.close();
}
QJsonDocument WeatherLocation::convertToJson(AbstractWeatherForecast &fc)
{
    QJsonDocument doc;
    doc.setObject(fc.toJson());
    return doc;
}

void WeatherLocation::changeBackend(Kweather::Backend backend)
{
    if (backend != backend_) {
        auto old = weatherBackendProvider_;
        backend_ = backend;
        AbstractWeatherAPI *tmp = nullptr;
        switch (backend_) {
        case Kweather::Backend::OWM:
            tmp = new OWMWeatherAPI(this->locationId(), this->timeZone(), this->latitude_, this->longitude());
            break;
        case Kweather::Backend::NMI:
            tmp = new NMIWeatherAPI2(this->locationId(), this->timeZone(), this->latitude_, this->longitude());
            break;
        default:
            return;
        }
        weatherBackendProvider_ = tmp;
        connect(this->weatherBackendProvider(), &AbstractWeatherAPI::updated, this, &WeatherLocation::updateData, Qt::UniqueConnection);
        weatherBackendProvider_->setCurrentSunriseData(old->currentSunriseData());
        weatherBackendProvider_->fetchSunriseData();
        this->update();
        old->deleteLater();
    }
}

WeatherLocation::~WeatherLocation()
{
    delete weatherBackendProvider_;
}

/* ~~~ WeatherLocationListModel ~~~ */
WeatherLocationListModel::WeatherLocationListModel(QObject *parent)
{
    load();

#ifndef Q_OS_ANDROID
    QDBusConnection::sessionBus().registerObject("/", this, QDBusConnection::ExportScriptableContents);
#endif
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

#ifndef Q_OS_ANDROID
    for (auto loc : this->locationsList) {
        QDBusConnection::sessionBus().registerObject("/locations/" + loc->locationId(), loc, QDBusConnection::ExportScriptableContents);
        Q_EMIT added(loc->locationId());
    }
#endif
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
    Q_EMIT removed(location->locationId());
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

#ifndef Q_OS_ANDROID
        QDBusConnection::sessionBus().registerObject("/locations/" + location->locationId(), location, QDBusConnection::ExportScriptableContents);
        Q_EMIT added(location->locationId());
#endif
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

#ifndef Q_OS_ANDROID
    QDBusConnection::sessionBus().registerObject("/locations/" + location->locationId(), location, QDBusConnection::ExportScriptableContents);
    Q_EMIT added(location->locationId());
#endif
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
