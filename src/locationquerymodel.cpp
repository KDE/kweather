/*
 * Copyright 2020 2021 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "locationquerymodel.h"
#include <QDebug>
#include <QTimer>
LocationQueryModel::LocationQueryModel()
{
    inputTimer = new QTimer(this);
    inputTimer->setSingleShot(true);
    connect(inputTimer, &QTimer::timeout, this, &LocationQueryModel::setQuery);
    connect(&m_querySource, &KWeatherCore::LocationQuery::queryFinished, this, &LocationQueryModel::handleQueryResults);
}

int LocationQueryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_results.size();
}

QVariant LocationQueryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto result = m_results.at(index.row());

    if (role == NameRole) {
        return result.toponymName() + ", " + result.countryName();
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
    m_text = query;

    Q_EMIT layoutAboutToBeChanged();
    // clear results list
    m_results.clear();

    Q_EMIT layoutChanged();
    if (!query.isEmpty()) { // do not query nothing
        m_loading = true;
        m_networkError = false;
        Q_EMIT propertyChanged();

        inputTimer->start(timeout); // make request once input stopped for 2 secs
    }
}

void LocationQueryModel::setQuery()
{
    m_querySource.query(m_text);
}

void LocationQueryModel::addLocation(int index)
{
    if (m_results.empty() || index < 0 || index >= static_cast<int>(m_results.size()))
        return; // don't add location
    Q_EMIT appendLocation(m_results.at(index));
}

void LocationQueryModel::handleQueryResults(const std::vector<KWeatherCore::LocationQueryResult> &results)
{
    qDebug() << "results arrived" << results.size();
    Q_EMIT layoutAboutToBeChanged();
    // clear results list
    m_results.assign(results.begin(), results.end());

    Q_EMIT layoutChanged();
}

void LocationQueryModel::updateUi()
{
}
bool LocationQueryModel::loading() const
{
    return m_loading;
}
bool LocationQueryModel::networkError() const
{
    return m_networkError;
}
