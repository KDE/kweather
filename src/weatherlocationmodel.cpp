/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "weatherlocationmodel.h"
#include "weatherdaymodel.h"
#include "weatherlocation.h"

#include <KConfigCore/KConfigGroup>
#include <KConfigCore/KSharedConfig>

#include <QJsonArray>
#include <QQmlEngine>

const QString WEATHER_LOCATIONS_CFG_GROUP = QStringLiteral("WeatherLocations");
const QString WEATHER_LOCATIONS_CFG_KEY = QStringLiteral("m_locations");

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
        m_locations.push_back(WeatherLocation::fromJson(obj));
    }
}

void WeatherLocationListModel::save()
{
    QJsonArray arr;
    for (const auto &lc : qAsConst(m_locations)) {
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
    return m_locations.size();
}

QVariant WeatherLocationListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= static_cast<int>(m_locations.size()) || index.row() < 0) {
        return {};
    }
    if (role == Roles::LocationRole) {
        return QVariant::fromValue(m_locations.at(index.row()));
    }
    return {};
}

QHash<int, QByteArray> WeatherLocationListModel::roleNames() const
{
    return {{Roles::LocationRole, "location"}};
}

void WeatherLocationListModel::updateUi()
{
    Q_EMIT dataChanged(createIndex(0, 0), createIndex(m_locations.size() - 1, 0));
    for (const auto &l : qAsConst(m_locations)) {
        Q_EMIT l->propertyChanged();
        l->weatherDayListModel()->updateUi();
        l->weatherHourListModel()->updateUi();
    }
}

void WeatherLocationListModel::insert(int index, WeatherLocation *weatherLocation)
{
    if ((index < 0) || (index > static_cast<int>(m_locations.size())))
        return;

    QQmlEngine::setObjectOwnership(weatherLocation, QQmlEngine::CppOwnership);
    beginInsertRows(QModelIndex(), index, index);
    m_locations.insert(m_locations.begin() + index, weatherLocation);
    endInsertRows();

    save();
}

void WeatherLocationListModel::remove(int index)
{
    if ((index < 0) || (index >= static_cast<int>(m_locations.size())))
        return;

    beginRemoveRows(QModelIndex(), index, index);
    auto location = m_locations.at(index);
    m_locations.erase(m_locations.begin() + index);
    delete location;
    endRemoveRows();

    save();
}

WeatherLocation *WeatherLocationListModel::get(int index)
{
    if ((index < 0) || (index >= static_cast<int>(m_locations.size())))
        return {};

    return m_locations.at(index);
}

void WeatherLocationListModel::move(int oldIndex, int newIndex)
{
    int locationsSize = m_locations.size();
    if (oldIndex < 0 || oldIndex >= locationsSize || newIndex < 0 || newIndex >= locationsSize)
        return;

    // to my surprise, we have to do this
    if (newIndex > oldIndex)
        std::swap(newIndex, oldIndex);

    beginMoveRows(QModelIndex(), oldIndex, oldIndex, QModelIndex(), newIndex);
    std::iter_swap(m_locations.begin() + oldIndex, m_locations.begin() + newIndex);
    endMoveRows();
}
int WeatherLocationListModel::count() const
{
    return m_locations.size();
}
void WeatherLocationListModel::addLocation(const KWeatherCore::LocationQueryResult &ret)
{
    qDebug() << "add location";
    auto locId = ret.geonameId(), locName = ret.toponymName();
    auto lat = ret.latitude(), lon = ret.longitude();

    // add location
    auto *location = new WeatherLocation(locId, locName, QString(), lat, lon);
    location->update();

    insert(m_locations.size(), location);
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
