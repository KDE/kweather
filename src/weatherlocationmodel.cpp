/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "weatherlocationmodel.h"
#include "weatherlocation.h"

#include <KConfigCore/KConfigGroup>
#include <KConfigCore/KSharedConfig>

#include <QJsonArray>
#include <QQmlEngine>
/* ~~~ WeatherLocationListModel ~~~ */
WeatherLocationListModel::WeatherLocationListModel(QObject *parent)
    : QAbstractListModel(parent)
{
    load();
}

void WeatherLocationListModel::load()
{
    // load locations from kconfig
    auto config = KWeatherSettings(this).sharedConfig()->group(Kweather::WEATHER_LOCATIONS_CFG_GROUP);
    auto locations = config.groupList();
    for (const auto &location : locations) {
        auto location_ptr = WeatherLocation::load(location);
        if (location_ptr)
            m_locations.push_back(location_ptr);
    }

    auto i{0};
    for (auto loc : m_locations) {
        auto index = loc->index();
        while (index != i) {
            if (index != -1) {
                if (index >= (int)m_locations.size()) {
                    qDebug() << "invalid index, please delete config(~/.config/kweather/kweatherrc)";
                    abort();
                } else {
                    std::swap(m_locations[i], m_locations[index]);
                    index = m_locations[i]->index();
                }
            } else {
                loc->saveOrder(i);
                index = i;
            }
        }
        i++;
    }
}

void WeatherLocationListModel::saveOrder()
{
    auto i{0};
    for (auto loc : m_locations) {
        loc->saveOrder(i);
        i++;
    }
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

    saveOrder();
    weatherLocation->save();
}

void WeatherLocationListModel::remove(int index)
{
    if ((index < 0) || (index >= static_cast<int>(m_locations.size())))
        return;

    beginRemoveRows(QModelIndex(), index, index);
    auto location = m_locations.at(index);
    m_locations.erase(m_locations.begin() + index);
    location->deleteConfig();
    location->deleteLater();
    endRemoveRows();
    saveOrder();
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

    if (oldIndex < newIndex)
        ++newIndex;

    beginMoveRows(QModelIndex(), oldIndex, oldIndex, QModelIndex(), newIndex);
    std::iter_swap(m_locations.begin() + oldIndex, m_locations.begin() + newIndex);
    endMoveRows();

    saveOrder();
}
int WeatherLocationListModel::count() const
{
    return m_locations.size();
}
void WeatherLocationListModel::addLocation(const KWeatherCore::LocationQueryResult &ret)
{
    qDebug() << "add location";
    const auto &locId = ret.geonameId(), &locName = ret.toponymName();
    auto lat = ret.latitude(), lon = ret.longitude();

    // add location
    auto *location = new WeatherLocation(locId, locName, QString(), lat, lon);
    location->update();

    insert(m_locations.size(), location);
}

// invoked by frontend
void WeatherLocationListModel::requestCurrentLocation()
{
    static KWeatherCore::LocationQuery *geoPtr = nullptr;
    if (!geoPtr)
        geoPtr = new KWeatherCore::LocationQuery(this);

    geoPtr->locate();
    //    // failure
    connect(geoPtr, &KWeatherCore::LocationQuery::queryError, this, &WeatherLocationListModel::networkErrorCreatingDefault);
    //    // success
    connect(geoPtr, &KWeatherCore::LocationQuery::located, this, &WeatherLocationListModel::addCurrentLocation);
}

void WeatherLocationListModel::addCurrentLocation(const KWeatherCore::LocationQueryResult &ret)
{
    auto location = new WeatherLocation(ret.geonameId(), ret.toponymName(), QString(), ret.latitude(), ret.longitude());
    location->update();

    insert(0, location);
    Q_EMIT successfullyCreatedDefault();
}
