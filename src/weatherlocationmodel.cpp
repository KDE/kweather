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
#include "locationquerymodel.h"
#include "nmisunriseapi.h"
#include "nmiweatherapi2.h"
#include "owmweatherapi.h"
#include "weatherdaymodel.h"
#include "weatherhourmodel.h"

#include <KConfigCore/KConfigGroup>
#include <KConfigCore/KSharedConfig>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QQmlEngine>
#include <QTimeZone>

const QString WEATHER_LOCATIONS_CFG_GROUP = "WeatherLocations";
const QString WEATHER_LOCATIONS_CFG_KEY = "locationsList";

/* ~~~ WeatherLocation ~~~ */
WeatherLocation::WeatherLocation(AbstractWeatherForecast *forecast)
{
    this->weatherDayListModel_ = new WeatherDayListModel(this);
    this->weatherHourListModel_ = new WeatherHourListModel(this);
    this->lastUpdated_ = QDateTime::currentDateTime();
    this->forecast_ = forecast;
    QQmlEngine::setObjectOwnership(this->weatherDayListModel_,
                                   QQmlEngine::CppOwnership); // prevent segfaults from js garbage
                                                              // collecting
    QQmlEngine::setObjectOwnership(this->weatherHourListModel_, QQmlEngine::CppOwnership);
}

WeatherLocation::WeatherLocation(AbstractWeatherAPI *weatherBackendProvider,
                                 QString locationId,
                                 QString locationName,
                                 QString timeZone,
                                 float latitude,
                                 float longitude,
                                 Kweather::Backend backend,
                                 AbstractWeatherForecast *forecast)
    : weatherBackendProvider_(weatherBackendProvider)
    , locationId_(locationId)
    , locationName_(locationName)
    , latitude_(latitude)
    , longitude_(longitude)
    , forecast_(forecast)
    , backend_(backend)
{
    if (timeZone.isEmpty()) { // if we don't have timezone, get it
        geoTimeZone_ = new GeoTimeZone(latitude, longitude);
        connect(geoTimeZone_, &GeoTimeZone::finished, this, [this] {
            this->timeZone_ = geoTimeZone_->getTimeZone();
            weatherBackendProvider_->setTimeZone(&this->timeZone());
            if (!nmiSunriseApi_) {
                nmiSunriseApi_ =
                    new NMISunriseAPI(latitude_,
                                      longitude_,
                                      QDateTime::currentDateTime().toTimeZone(QTimeZone(QByteArray::fromStdString(timeZone_.toStdString()))).offsetFromUtc());
                connect(nmiSunriseApi_, &NMISunriseAPI::finished, this, &WeatherLocation::insertSunriseData);
            }
            emit timeZoneSet();
        });
    } else {
        timeZone_ = std::move(timeZone);
        nmiSunriseApi_ = new NMISunriseAPI(
            latitude_, longitude_, QDateTime::currentDateTime().toTimeZone(QTimeZone(QByteArray::fromStdString(timeZone_.toStdString()))).offsetFromUtc());
        connect(nmiSunriseApi_, &NMISunriseAPI::finished, this, &WeatherLocation::insertSunriseData);
    }
    this->weatherDayListModel_ = new WeatherDayListModel(this);
    this->weatherHourListModel_ = new WeatherHourListModel(this);
    this->lastUpdated_ = forecast == nullptr ? QDateTime::currentDateTime() : forecast->timeCreated();
    QQmlEngine::setObjectOwnership(this->weatherDayListModel_,
                                   QQmlEngine::CppOwnership); // prevent segfaults from js garbage
                                                              // collecting
    QQmlEngine::setObjectOwnership(this->weatherHourListModel_, QQmlEngine::CppOwnership);
    determineCurrentForecast();

    if (weatherBackendProvider != nullptr)
        weatherBackendProvider->setLocation(latitude, longitude);

    connect(this->weatherBackendProvider(), &AbstractWeatherAPI::updated, this, &WeatherLocation::updateData, Qt::UniqueConnection);
}

WeatherLocation *WeatherLocation::fromJson(const QJsonObject &obj)
{
    if (obj["backend"].toInt() == 0) {
        auto api = new NMIWeatherAPI2(obj["locationId"].toString());
        auto weatherLocation = new WeatherLocation(api,
                                                   obj["locationId"].toString(),
                                                   obj["locationName"].toString(),
                                                   obj["timezone"].toString(),
                                                   obj["latitude"].toDouble(),
                                                   obj["longitude"].toDouble());
        api->setTimeZone(&weatherLocation->timeZone_);
        return weatherLocation;
    } else {
        auto api = new OWMWeatherAPI(obj["locationID"].toString());
        auto weatherLocation = new WeatherLocation(api,
                                                   obj["locationId"].toString(),
                                                   obj["locationName"].toString(),
                                                   obj["timezone"].toString(),
                                                   obj["latitude"].toDouble(),
                                                   obj["longitude"].toDouble());
        api->setTimeZone(&weatherLocation->timeZone_);
        return weatherLocation;
    }
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

void WeatherLocation::updateData(AbstractWeatherForecast *fc)
{
    // only update if the forecast is newer
    if (forecast_ != nullptr && fc->timeCreated().toSecsSinceEpoch() < forecast_->timeCreated().toSecsSinceEpoch())
        return;
    if (fc->hourlyForecasts().at(0)->weatherIcon().isEmpty()) {      // if we don't have icon, prevent set icon twice when loading from cache
        if (sunriseList.count() != 0 && nmiSunriseApi_ != nullptr) { // if we have sunrise data
            for (auto hourForecast : fc->hourlyForecasts()) {
                hourForecast->setWeatherIcon(nmiSunriseApi_->isDayTime(hourForecast->date())); // set day/night icon
            }
        } else {
            for (auto hourForecast : fc->hourlyForecasts()) {
                if (hourForecast->date().time().hour() < 7 || hourForecast->date().time().hour() >= 18) // 6:00 - 18:00 is day
                    hourForecast->setWeatherIcon(false);
                else
                    hourForecast->setWeatherIcon(true);
            }
        }
    }
    forecast_ = fc; // don't need to delete pointers, they were already deleted
                    // by api class
    determineCurrentForecast();
    this->lastUpdated_ = fc->timeCreated();
    forecast_->setSunrise(sunriseList);
    emit weatherRefresh(forecast_);
    emit stopLoadingIndicator();
    writeToCache(forecast_);

    emit propertyChanged();
}

void WeatherLocation::determineCurrentForecast()
{
    if (forecast() == nullptr || forecast()->hourlyForecasts().count() == 0) {
        currentWeather_ = new AbstractHourlyWeatherForecast(
            QDateTime::currentDateTime(), "Unknown", "weather-none-available", "weather-none-available", 0, 0, Kweather::WindDirection::N, 0, 0, 0, 0, 0);
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
    QQmlEngine::setObjectOwnership(currentWeather_, QQmlEngine::CppOwnership); // prevent segfaults from js
                                                                               // garbage collecting
    emit currentForecastChange();
}

void WeatherLocation::initData(AbstractWeatherForecast *fc)
{
    forecast_ = fc;
    weatherBackendProvider_->setCurrentData(forecast_);
    nmiSunriseApi_->setData(fc->sunrise());
    sunriseList = nmiSunriseApi_->get();
    determineCurrentForecast();
    emit weatherRefresh(forecast_);
    emit propertyChanged();
}

void WeatherLocation::update()
{
    weatherBackendProvider_->update();
    if (nmiSunriseApi_) {
        nmiSunriseApi_->popDay();
        nmiSunriseApi_->update();
    }
}

void WeatherLocation::insertSunriseData()
{
    sunriseList = nmiSunriseApi_->get();
    if (forecast_) {
        forecast_->setSunrise(sunriseList);
        for (auto hourForecast : forecast_->hourlyForecasts()) {
            hourForecast->setWeatherIcon(nmiSunriseApi_->isDayTime(hourForecast->date())); // set day/night icon
        }
        emit weatherRefresh(forecast_);
        emit propertyChanged();
        writeToCache(forecast_);
    }
}

void WeatherLocation::writeToCache(AbstractWeatherForecast *fc)
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
QJsonDocument WeatherLocation::convertToJson(AbstractWeatherForecast *fc)
{
    QJsonDocument doc;
    doc.setObject(fc->toJson());
    return doc;
}

void WeatherLocation::changeBackend(Kweather::Backend backend)
{
    if (backend != backend_) {
        backend_ = backend;
        AbstractWeatherAPI *tmp = nullptr;
        switch (backend_) {
        case Kweather::Backend::OWM:
            tmp = new OWMWeatherAPI(weatherBackendProvider_->locationName());
            delete weatherBackendProvider_;
            weatherBackendProvider_ = tmp;
            this->update();
            break;
        case Kweather::Backend::NMI:
            tmp = new NMIWeatherAPI2(weatherBackendProvider_->locationName());
            delete weatherBackendProvider_;
            weatherBackendProvider_ = tmp;
            this->update();
            break;
        default:
            return;
        }
    }
}

WeatherLocation::~WeatherLocation()
{
    if (nmiSunriseApi_ != nullptr)
        delete nmiSunriseApi_;
    if (weatherBackendProvider_ != nullptr)
        delete weatherBackendProvider_;
    if (geoTimeZone_ != nullptr)
        delete geoTimeZone_;
}

/* ~~~ WeatherLocationListModel ~~~ */
WeatherLocationListModel::WeatherLocationListModel(QObject *parent)
{
    load();
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
    if (!weatherLocation->timeZone().isEmpty())
        save();
    else
        connect(
            weatherLocation, &WeatherLocation::timeZoneSet, this, [this] { this->save(); }, Qt::UniqueConnection);
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
    auto api = new NMIWeatherAPI2(ret->geonameId());
    qDebug() << "lat" << ret->latitude();
    qDebug() << "lgn" << ret->longitude();
    api->setLocation(ret->latitude(), ret->longitude());
    auto location = new WeatherLocation(api, ret->geonameId(), ret->name(), QString(), ret->latitude(), ret->longitude());
    location->update();
    insert(this->locationsList.count(), location);
}

void WeatherLocationListModel::requestCurrentLocation()
{
    geoPtr = new GeoIPLookup();
    connect(geoPtr, &GeoIPLookup::finished, this, &WeatherLocationListModel::addCurrentLocation);
}

void WeatherLocationListModel::addCurrentLocation()
{
    // default location, use timestamp as id
    long id = QDateTime::currentSecsSinceEpoch();
    auto api = new NMIWeatherAPI2(QString::number(id));
    api->setLocation(geoPtr->latitude(), geoPtr->longitude());
    auto location = new WeatherLocation(api, QString::number(id), geoPtr->name(), geoPtr->timeZone(), geoPtr->latitude(), geoPtr->longitude());
    api->setTimeZone(&location->timeZone());
    location->update();
    insert(0, location);
}
