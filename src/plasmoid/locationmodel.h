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
    enum LocationRole {LocationName = Qt::UserRole + 1, Temperature, Icon, Description, Precipitation, Humidity, Date};
    LocationModel();
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual int rowCount(const QModelIndex &index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
    const QString &getLocation(int index) const;
private Q_SLOTS:
    void update();

protected:
    friend class KWeather_1x4;
    QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> getData(int index) const;
private:
    class Location;

    QTimer *m_timer = nullptr;
    std::vector<Location *> m_locations;
};

#endif // LOCATIONMODEL_H
