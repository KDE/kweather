/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <qqmlregistration.h>

#include <KWeatherCore/LocationQuery>

class QTimer;
class LocationQueryModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(bool loading READ loading NOTIFY propertyChanged)
    Q_PROPERTY(bool networkError READ networkError NOTIFY propertyChanged)

public:
    explicit LocationQueryModel();
    enum Roles {
        NameRole = Qt::DisplayRole,
    };

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE bool loading() const;
    Q_INVOKABLE bool networkError() const;
    Q_INVOKABLE void textChanged(QString query, int timeout = 2000);
    Q_INVOKABLE void addLocation(int index);
    Q_INVOKABLE KWeatherCore::LocationQueryResult get(int index);
    Q_INVOKABLE void clearResults();
    void setQuery();
Q_SIGNALS:
    void propertyChanged();
    void appendLocation(const KWeatherCore::LocationQueryResult &result);
private Q_SLOTS:
    void handleQueryResults(const std::vector<KWeatherCore::LocationQueryResult> &results);

private:
    bool m_loading = false, m_networkError = false;
    std::vector<KWeatherCore::LocationQueryResult> m_results;
    KWeatherCore::LocationQuery m_querySource;
    QTimer *const inputTimer;
    QString m_text;
};
