/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "locationquerymodel.h"
#include "weatherlocationlistmodel.h"

#include <KWeatherCore/LocationQueryReply>

#include <QDebug>
#include <QTimer>

LocationQueryModel::LocationQueryModel()
    : inputTimer(new QTimer(this))
{
    inputTimer->setSingleShot(true);
    connect(inputTimer, &QTimer::timeout, this, &LocationQueryModel::setQuery);
}

int LocationQueryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_results.size();
}

static QString buildResultName(const KWeatherCore::LocationQueryResult &result)
{
    const auto &countryCode = result.countryCode();
    if (result.subdivision() && countryCode == QLatin1String("US")) {
        return result.toponymName() + QLatin1String(", ") + *result.subdivision() + QLatin1String(" | ") + result.countryName();
    }
    return result.toponymName() + QLatin1String(" | ") + result.countryName();
}

QVariant LocationQueryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto result = m_results.at(index.row());

    if (role == NameRole) {
        return buildResultName(result);
    }

    return QVariant();
}

QHash<int, QByteArray> LocationQueryModel::roleNames() const
{
    return {{NameRole, "name"}};
}

KWeatherCore::LocationQueryResult LocationQueryModel::get(int index)
{
    if (index < 0 || index >= static_cast<int>(m_results.size()))
        return {};
    return m_results.at(index);
}

void LocationQueryModel::textChanged(QString query, int timeout)
{
    m_text = std::move(query);

    beginResetModel();
    // clear results list
    m_results.clear();

    endResetModel();
    if (!m_text.isEmpty()) { // do not query nothing
        m_loading = true;
        m_networkError = false;
        Q_EMIT propertyChanged();

        inputTimer->start(timeout); // make request once input stopped for 2 secs
    }
}

void LocationQueryModel::setQuery()
{
    qDebug() << "start query";
    auto reply = m_querySource.query(m_text);
    connect(reply, &KWeatherCore::LocationQueryReply::finished, this, [this, reply]() {
        reply->error();
        if (reply->error() == KWeatherCore::LocationQueryReply::NoError || reply->error() == KWeatherCore::LocationQueryReply::NotFound) {
            handleQueryResults(reply->result());
        }
    });
}

void LocationQueryModel::addLocation(int index)
{
    if (m_results.empty() || index < 0 || index >= static_cast<int>(m_results.size()))
        return; // don't add location
    WeatherLocationListModel::inst()->addLocation(m_results.at(index));
}

void LocationQueryModel::handleQueryResults(const std::vector<KWeatherCore::LocationQueryResult> &results)
{
    qDebug() << "results arrived" << results.size();
    beginResetModel();
    // clear results list
    m_results.assign(results.begin(), results.end());

    endResetModel();
}

bool LocationQueryModel::loading() const
{
    return m_loading;
}
bool LocationQueryModel::networkError() const
{
    return m_networkError;
}

void LocationQueryModel::clearResults()
{
    beginResetModel();
    m_results.clear();
    endResetModel();
    m_loading = false;
    m_networkError = false;
    Q_EMIT propertyChanged();
}

#include "moc_locationquerymodel.cpp"
