/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "locationquerymodel.h"
#include <QDebug>
#include <QTimer>
class LocationQueryResult : public QObject
{
    Q_OBJECT

public:
    explicit LocationQueryResult(const KWeatherCore::LocationQueryResult &result)
        : d(result)
    {
    }

    double latitude() const
    {
        return d.latitude();
    }
    double longitude() const
    {
        return d.longitude();
    }
    QString toponymName()
    {
        return d.toponymName();
    }
    QString name()
    {
        return d.name();
    }
    QString countryCode()
    {
        return d.countryCode();
    }
    QString countryName()
    {
        return d.countryName();
    }
    QString geonameId()
    {
        return d.geonameId();
    }

private:
    friend class LocationQueryModel;
    KWeatherCore::LocationQueryResult d;
};

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
    return resultsVec.size();
}

QVariant LocationQueryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto query = resultsVec[index.row()];

    if (role == NameRole) {
        return query->toponymName() + ", " + query->countryName();
    }

    return QVariant();
}

QHash<int, QByteArray> LocationQueryModel::roleNames() const
{
    return {{NameRole, "name"}};
}

LocationQueryResult *LocationQueryModel::get(int index)
{
    if (index < 0 || index >= resultsVec.count())
        return {};
    return resultsVec.at(index);
}

void LocationQueryModel::textChanged(QString query, int timeout)
{
    m_text = query;

    Q_EMIT layoutAboutToBeChanged();
    // clear results list
    resultsVec.clear();

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
    if (resultsVec.size() == 0 || index < 0 || index >= resultsVec.size())
        return; // don't add location
    Q_EMIT appendLocation(&resultsVec.at(index)->d);
}

void LocationQueryModel::handleQueryResults(QVector<KWeatherCore::LocationQueryResult> result)
{
    qDebug() << "results arrived" << result.size();
    Q_EMIT layoutAboutToBeChanged();
    // clear results list
    resultsVec.clear();
    for (const auto &ret : result)
        resultsVec.append(new LocationQueryResult(ret));

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
#include "locationquerymodel.moc"
