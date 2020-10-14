/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef WEATHERLOCATIONMODEL_H
#define WEATHERLOCATIONMODEL_H

#include "abstractweatherforecast.h"
#include "nmiweatherapi2.h"
#include "weatherhourmodel.h"

#include <QAbstractListModel>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <utility>

class LocationQueryResult;
class GeoIPLookup;
class WeatherLocation;
class WeatherLocationListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit WeatherLocationListModel(QObject *parent = nullptr);

    enum Roles { LocationRole = Qt::UserRole };

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void updateUi();
    void load();
    void save();
    Q_INVOKABLE void insert(int index, WeatherLocation *weatherLocation);
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void move(int oldIndex, int newIndex);
    Q_INVOKABLE int count()
    {
        return locationsList.count();
    }
    Q_INVOKABLE WeatherLocation *get(int index);
    inline QList<WeatherLocation *> &getList()
    {
        return locationsList;
    };
    Q_INVOKABLE void requestCurrentLocation();
    Q_INVOKABLE void changeBackend(int index, QString backend);
public slots:
    void addLocation(LocationQueryResult *ret);
signals:
    void networkErrorCreating();        // error creating a location
    void networkErrorCreatingDefault(); // error getting current location
    void successfullyCreatedDefault();  // successful in getting current location

private:
    void addCurrentLocation();
    GeoIPLookup *geoPtr = nullptr;
    QList<WeatherLocation *> locationsList;
};

#endif // WEATHERLOCATIONMODEL_H
