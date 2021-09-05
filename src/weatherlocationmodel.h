/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QAbstractListModel>
#include <QObject>

#include <KWeatherCore/LocationQuery>

class WeatherLocation;
class WeatherLocationListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY locationsChanged)
    Q_PROPERTY(QList<WeatherLocation *> locations READ locations NOTIFY locationsChanged)

    Q_PROPERTY(bool isLowPower READ isLowPower CONSTANT)

public:
    enum Roles { LocationRole = Qt::UserRole };

    static WeatherLocationListModel *inst();

    void load();
    void saveOrder();
    Q_INVOKABLE void insert(int index, WeatherLocation *weatherLocation);
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void move(int oldIndex, int newIndex);
    int count() const;
    Q_INVOKABLE void requestCurrentLocation();
    QList<WeatherLocation *> &locations();

    bool isLowPower();

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
