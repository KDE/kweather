/*
    SPDX-FileCopyrightText: 2021 HanYoung <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/
#ifndef LOCATIONMODEL_H
#define LOCATIONMODEL_H

#include <QAbstractListModel>
#include <KWeatherCore/WeatherForecast>
class QTimer;
class LocationModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum LocationRole {LocationName, Temperature, Icon, Description, Precipitation};
    LocationModel();
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual int rowCount(const QModelIndex &index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
private Q_SLOTS:
    void update();
    void updateData(QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> forecast);
private:
    class Location;

    QTimer *m_timer = nullptr;
    std::vector<Location *> m_locations;
};

#endif // LOCATIONMODEL_H
