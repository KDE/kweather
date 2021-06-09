/*
    SPDX-FileCopyrightText: 2021 HanYoung <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/
#ifndef HOURLYMODEL_H
#define HOURLYMODEL_H

#include <QAbstractListModel>
#include <KWeatherCore/WeatherForecast>
class HourlyModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum HourlyRole {Time = Qt::UserRole + 1, Icon, Description, Temperature, Precipitation};
    HourlyModel();
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual int rowCount(const QModelIndex &index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
    void loadForecast(QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast>);
private:
    const KWeatherCore::HourlyWeatherForecast &getNthHour(int index) const;
    QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> m_location;
};

#endif // HOURLYNMODEL_H
