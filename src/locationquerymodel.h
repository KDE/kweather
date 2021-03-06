/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <KWeatherCore/LocationQuery>
#include <QAbstractListModel>
#include <QObject>
class QTimer;
class LocationQueryModel : public QAbstractListModel
{
    Q_OBJECT
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
    Q_INVOKABLE void updateUi();
    Q_INVOKABLE KWeatherCore::LocationQueryResult get(int index);
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
    QTimer *inputTimer = nullptr;
    QString m_text;
};
