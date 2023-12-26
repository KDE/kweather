// SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
// SPDX-FileCopyrightText: 2020-2022 Devin Lin <espidev@gmail.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "weatherlocationlistmodel.h"

#include <QJsonArray>
#include <QQmlEngine>

#include <KConfigGroup>
#include <KSharedConfig>

#include <KWeatherCore/LocationQueryReply>

#include "global.h"
#include "kweathersettings.h"
#include "weatherlocation.h"

WeatherLocationListModel::WeatherLocationListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    load();
}

WeatherLocationListModel *WeatherLocationListModel::create(QQmlEngine *, QJSEngine *)
{
    QQmlEngine::setObjectOwnership(inst(), QQmlEngine::CppOwnership);
    return inst();
}

WeatherLocationListModel *WeatherLocationListModel::inst()
{
    static WeatherLocationListModel *singleton = new WeatherLocationListModel;
    return singleton;
}

void WeatherLocationListModel::load()
{
    beginResetModel();

    // load locations from kconfig
    auto config = KWeatherSettings::self()->config()->group(KWeather::WEATHER_LOCATIONS_CFG_GROUP);
    auto locations = config.groupList();
    for (const auto &location : locations) {
        auto location_ptr = WeatherLocation::load(location);
        if (location_ptr)
            m_locations.push_back(location_ptr);
    }

    // sort locations by index, correcting any issues with the stored index

    QList<WeatherLocation *> sorted, unsorted;
    for (int i = 0; i < (int)m_locations.size(); ++i) {
        sorted.push_back(nullptr);
    }

    // loop through the initial locations and fill in the indicies in sorted
    for (auto loc : m_locations) {
        auto index = loc->index();

        if (index < 0 || index >= (int)sorted.size() || sorted[index] != nullptr) {
            unsorted.push_back(loc);
        } else {
            sorted[index] = loc;
        }
    }
    // add unsorted locations in positions unfilled
    for (auto loc : unsorted) {
        for (int i = 0; i < (int)sorted.size(); ++i) {
            if (!sorted[i]) {
                sorted[i] = loc;
                break;
            }
        }
    }
    // move into original array
    for (int i = 0; i < (int)m_locations.size(); ++i) {
        m_locations[i] = sorted[i];
    }

    Q_EMIT locationsChanged();
    endResetModel();
}

void WeatherLocationListModel::saveOrder()
{
    auto i{0};
    for (auto loc : m_locations) {
        loc->saveOrder(i);
        i++;
    }
}

int WeatherLocationListModel::count() const
{
    return m_locations.size();
}

int WeatherLocationListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return m_locations.size();
}

QVariant WeatherLocationListModel::data(const QModelIndex &index, int role) const
{
    if (!checkIndex(index)) {
        return QVariant();
    }
    if (role != LocationRole) {
        return QVariant();
    }

    auto *location = m_locations[index.row()];
    return location ? QVariant::fromValue(location) : QVariant();
}

QHash<int, QByteArray> WeatherLocationListModel::roleNames() const
{
    return {{LocationRole, "location"}};
}

void WeatherLocationListModel::insert(int index, WeatherLocation *weatherLocation)
{
    if ((index < 0) || (index > static_cast<int>(m_locations.size()))) {
        return;
    }

    beginInsertRows(QModelIndex(), index, index);

    QQmlEngine::setObjectOwnership(weatherLocation, QQmlEngine::CppOwnership);
    m_locations.insert(m_locations.begin() + index, weatherLocation);

    Q_EMIT locationsChanged();
    endInsertRows();

    saveOrder();
    weatherLocation->save();
}

void WeatherLocationListModel::remove(int index)
{
    if ((index < 0) || (index >= static_cast<int>(m_locations.size()))) {
        return;
    }

    beginRemoveRows(QModelIndex(), index, index);

    auto location = m_locations.at(index);
    m_locations.erase(m_locations.begin() + index);
    location->deleteConfig();
    location->deleteLater();

    Q_EMIT locationsChanged();
    endRemoveRows();

    saveOrder();
}

void WeatherLocationListModel::move(int oldIndex, int newIndex)
{
    int locationsSize = m_locations.size();
    if (oldIndex < 0 || oldIndex >= locationsSize || newIndex < 0 || newIndex >= locationsSize) {
        return;
    }
    if (newIndex > oldIndex) {
        ++newIndex;
    }

    beginMoveRows(QModelIndex(), oldIndex, oldIndex, QModelIndex(), newIndex);
    if (newIndex > oldIndex) {
        auto *location = m_locations.at(oldIndex);
        m_locations.insert(newIndex, location);
        m_locations.takeAt(oldIndex);
    } else {
        auto *location = m_locations.takeAt(oldIndex);
        m_locations.insert(newIndex, location);
    }
    endMoveRows();
    Q_EMIT locationsChanged();

    saveOrder();
}

void WeatherLocationListModel::addLocation(const KWeatherCore::LocationQueryResult &ret)
{
    const auto &locId = ret.geonameId();
    const auto &locName = ret.toponymName();
    auto lat = ret.latitude();
    auto lon = ret.longitude();

    // add location
    auto *location = new WeatherLocation(locId, locName, QString(), lat, lon);

    insert(m_locations.size(), location);
}

void WeatherLocationListModel::requestCurrentLocation()
{
    static KWeatherCore::LocationQuery *geoPtr = nullptr;
    if (!geoPtr) {
        geoPtr = new KWeatherCore::LocationQuery(this);
    }

    auto reply = geoPtr->locate();
    connect(reply, &KWeatherCore::LocationQueryReply::finished, this, [reply, this]() {
        reply->deleteLater();
        if (reply->error() != KWeatherCore::LocationQueryReply::NoError) {
            Q_EMIT networkErrorCreatingDefault();
        } else {
            addCurrentLocation(reply->result().at(0));
        }
    });
}

void WeatherLocationListModel::addCurrentLocation(const KWeatherCore::LocationQueryResult &ret)
{
    auto location = new WeatherLocation(ret.geonameId(), ret.toponymName(), QString(), ret.latitude(), ret.longitude());

    insert(0, location);
    Q_EMIT successfullyCreatedDefault();
}

QList<WeatherLocation *> &WeatherLocationListModel::locations()
{
    return m_locations;
}

#include "moc_weatherlocationlistmodel.cpp"
