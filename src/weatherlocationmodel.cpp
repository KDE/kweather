/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "weatherlocationmodel.h"
#include "weatherlocation.h"
#include "weatherdaymodel.h"

#include <KConfigCore/KConfigGroup>
#include <KConfigCore/KSharedConfig>

#include <QJsonArray>
#include <QQmlEngine>

const QString WEATHER_LOCATIONS_CFG_GROUP = QStringLiteral("WeatherLocations");
const QString WEATHER_LOCATIONS_CFG_KEY = QStringLiteral("locationsVec");

/* ~~~ WeatherLocationListModel ~~~ */
WeatherLocationListModel::WeatherLocationListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    load();
}

void WeatherLocationListModel::load()
{
    // load locations from kconfig
    auto config = KSharedConfig::openConfig(QString(), KSharedConfig::FullConfig, QStandardPaths::AppConfigLocation);
    KConfigGroup group = config->group(WEATHER_LOCATIONS_CFG_GROUP);
    QJsonDocument doc = QJsonDocument::fromJson(group.readEntry(WEATHER_LOCATIONS_CFG_KEY, "{}").toUtf8());
    const auto &array = doc.array();
    for (const auto &r : array) {
        QJsonObject obj = r.toObject();
        locationsVec.append(WeatherLocation::fromJson(obj));
    }
}

void WeatherLocationListModel::save()
{
    QJsonArray arr;
    for (const auto &lc : qAsConst(locationsVec)) {
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
    return locationsVec.size();
}

QVariant WeatherLocationListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= locationsVec.size() || index.row() < 0) {
        return {};
    }
    if (role == Roles::LocationRole) {
        return QVariant::fromValue(locationsVec.at(index.row()));
    }
    return {};
}

QHash<int, QByteArray> WeatherLocationListModel::roleNames() const
{
    return {{Roles::LocationRole, "location"}};
}

void WeatherLocationListModel::updateUi()
{
    Q_EMIT dataChanged(createIndex(0, 0), createIndex(locationsVec.size() - 1, 0));
    for (const auto &l : qAsConst(locationsVec)) {
        Q_EMIT l->propertyChanged();
        l->weatherDayListModel()->updateUi();
        l->weatherHourListModel()->updateUi();
    }
}

void WeatherLocationListModel::insert(int index, WeatherLocation *weatherLocation)
{
    if ((index < 0) || (index > locationsVec.size()))
        return;

    QQmlEngine::setObjectOwnership(weatherLocation, QQmlEngine::CppOwnership);
    beginInsertRows(QModelIndex(), index, index);
    locationsVec.insert(index, weatherLocation);
    endInsertRows();

    save();
}

void WeatherLocationListModel::remove(int index)
{
    if ((index < 0) || (index >= locationsVec.size()))
        return;

    beginRemoveRows(QModelIndex(), index, index);
    auto location = locationsVec.at(index);
    locationsVec.removeAt(index);
    delete location;
    endRemoveRows();

    save();
}

WeatherLocation *WeatherLocationListModel::get(int index)
{
    if ((index < 0) || (index >= locationsVec.size()))
        return {};

    return locationsVec.at(index);
}

void WeatherLocationListModel::move(int oldIndex, int newIndex)
{
    if (oldIndex < 0 || oldIndex >= locationsVec.size() || newIndex < 0 || newIndex >= locationsVec.size())
        return;

    // to my surprise, we have to do this
    if (newIndex > oldIndex)
        std::swap(newIndex, oldIndex);

    beginMoveRows(QModelIndex(), oldIndex, oldIndex, QModelIndex(), newIndex);
    locationsVec.move(oldIndex, newIndex);
    endMoveRows();
}
int WeatherLocationListModel::count() const
{
    return locationsVec.size();
}
void WeatherLocationListModel::addLocation(KWeatherCore::LocationQueryResult *ret)
{
    qDebug() << "add location";
    auto locId = ret->geonameId(), locName = ret->toponymName();
    auto lat = ret->latitude(), lon = ret->longitude();

    // add location
    auto *location = new WeatherLocation(locId, locName, QString(), lat, lon);
    location->update();

    insert(this->locationsVec.size(), location);
}

// invoked by frontend
void WeatherLocationListModel::requestCurrentLocation()
{
    auto geoPtr = new KWeatherCore::LocationQuery(this);
    geoPtr->locate();
    //    // failure
    //    connect(geoPtr, &KWeatherCore::LocationQuery::, this, [this]() { Q_EMIT networkErrorCreatingDefault(); });
    //    // success
    connect(geoPtr, &KWeatherCore::LocationQuery::located, this, &WeatherLocationListModel::addCurrentLocation);
}

void WeatherLocationListModel::addCurrentLocation(KWeatherCore::LocationQueryResult ret)
{
    auto location = new WeatherLocation(ret.geonameId(), ret.name(), QString(), ret.latitude(), ret.longitude());
    location->update();

    insert(0, location);
    Q_EMIT successfullyCreatedDefault();
}

QVector<WeatherLocation *> &WeatherLocationListModel::getVec()
{
    return locationsVec;
}
