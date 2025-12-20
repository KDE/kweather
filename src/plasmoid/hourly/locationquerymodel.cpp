/*
 * SPDX-FileCopyrightText: 2020-2021 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2025 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "locationquerymodel.h"
#include "weatherlocationlistmodel.h"

#include <KWeatherCore/LocationQueryReply>

#include <QDebug>
#include <QTimer>

LocationQueryModel::LocationQueryModel(QObject *parent)
    : QAbstractListModel(parent)
    , m_inputTimer(new QTimer(this))
{
    m_inputTimer->setSingleShot(true);
    connect(m_inputTimer, &QTimer::timeout, this, &LocationQueryModel::setQuery);
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
    if (!index.isValid() || index.row() > m_results.size() || index.row() < 0)
        return QVariant();

    auto result = m_results.at(index.row());

    switch (role) {
    case NameRole:
        return buildResultName(result);
    case LatitudeRole:
        return result.latitude();
    case LongitudeRole:
        return result.longitude();
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> LocationQueryModel::roleNames() const
{
    return {{NameRole, "name"}, {LatitudeRole, "latitude"}, {LongitudeRole, "longitude"}};
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

        m_inputTimer->start(timeout); // make request once input stopped for 2 secs
    }
}

void LocationQueryModel::setQuery()
{
    m_loading = true;
    m_networkError = false;
    Q_EMIT propertyChanged();

    qDebug() << "start query";

    auto reply = m_querySource.query(m_text);
    connect(reply, &KWeatherCore::LocationQueryReply::finished, this, [this, reply]() {
        m_loading = false;

        qDebug() << "got query response";

        // Handling errors
        auto err = reply->error();
        if (err == KWeatherCore::LocationQueryReply::NoError || err == KWeatherCore::LocationQueryReply::NotFound) {
            handleQueryResults(reply->result());
        } else if (err == KWeatherCore::LocationQueryReply::NetworkError) {
            qDebug() << "network error";
            m_networkError = true;
            Q_EMIT propertyChanged();
        }

        reply->deleteLater();
    });
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

#include "moc_locationquerymodel.cpp"
