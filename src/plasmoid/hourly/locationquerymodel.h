/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2025 Devin Lin <espidev@gmail.com>
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
    explicit LocationQueryModel(QObject *parent = nullptr);

    enum Roles {
        NameRole = Qt::DisplayRole,
        LatitudeRole,
        LongitudeRole,
    };

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE bool loading() const;
    Q_INVOKABLE bool networkError() const;
    Q_INVOKABLE void textChanged(QString query, int timeout = 2000);
    void setQuery();

Q_SIGNALS:
    void propertyChanged();

private Q_SLOTS:
    void handleQueryResults(const std::vector<KWeatherCore::LocationQueryResult> &results);

private:
    bool m_loading = false, m_networkError = false;
    QList<KWeatherCore::LocationQueryResult> m_results;
    KWeatherCore::LocationQuery m_querySource;
    QTimer *m_inputTimer = nullptr;
    QString m_text;
};
