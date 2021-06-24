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

class WeatherLocation;
class WeatherLocationListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles { LocationRole = Qt::UserRole };

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void load();
    void saveOrder();
    Q_INVOKABLE void updateUi();
    Q_INVOKABLE void insert(int index, WeatherLocation *weatherLocation);
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE void move(int oldIndex, int newIndex);
    Q_INVOKABLE int count() const;
    Q_INVOKABLE WeatherLocation *get(int index);
    Q_INVOKABLE void requestCurrentLocation();
public Q_SLOTS:
    void addLocation(const KWeatherCore::LocationQueryResult &ret);
Q_SIGNALS:
    void networkErrorCreating(); // error creating a location
    void networkErrorCreatingDefault(); // error getting current location
    void successfullyCreatedDefault(); // successful in getting current location
protected:
    friend class WeatherForecastManager;
    explicit WeatherLocationListModel(QObject *parent = nullptr);
    std::vector<WeatherLocation *> &locations()
    {
        return m_locations;
    }
private Q_SLOTS:
    void addCurrentLocation(KWeatherCore::LocationQueryResult ret);

private:
    std::vector<WeatherLocation *> m_locations;
};
