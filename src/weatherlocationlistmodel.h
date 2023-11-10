// SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
// SPDX-FileCopyrightText: 2020-2022 Devin Lin <espidev@gmail.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>

#include <KWeatherCore/LocationQuery>

#include "weatherlocation.h"

class WeatherLocationListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    Q_PROPERTY(int count READ count NOTIFY locationsChanged)
    Q_PROPERTY(QList<WeatherLocation *> locations READ locations NOTIFY locationsChanged)

public:
    enum Roles { LocationRole = Qt::UserRole };

    static WeatherLocationListModel *inst();
    static WeatherLocationListModel *create(QQmlEngine *, QJSEngine *);

    void load();
    void saveOrder();

    int count() const;
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void insert(int index, WeatherLocation *weatherLocation);
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void move(int oldIndex, int newIndex);
    Q_INVOKABLE void requestCurrentLocation(); // invoked by UI

    QList<WeatherLocation *> &locations();

public Q_SLOTS:
    void addLocation(const KWeatherCore::LocationQueryResult &ret);

Q_SIGNALS:
    void locationsChanged();
    void networkErrorCreating(); // error creating a location
    void networkErrorCreatingDefault(); // error getting current location
    void successfullyCreatedDefault(); // successful in getting current location

protected:
    explicit WeatherLocationListModel(QObject *parent = nullptr);

private Q_SLOTS:
    void addCurrentLocation(const KWeatherCore::LocationQueryResult &ret);

private:
    QList<WeatherLocation *> m_locations;
};
