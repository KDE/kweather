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
    Q_PROPERTY(QString currentIcon READ currentIcon NOTIFY reseted)
    Q_PROPERTY(QString currentTemperature READ currentTemperature NOTIFY reseted)
    Q_PROPERTY(QString currentDescription READ currentDescription NOTIFY reseted)
public:
    enum HourlyRole { Time = Qt::UserRole + 1, Icon, Description, Temperature, Precipitation };
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &index) const override;
    QHash<int, QByteArray> roleNames() const override;
    const QString &currentIcon() const;
    QString currentTemperature() const;
    const QString &currentDescription() const;
    Q_INVOKABLE void openKWeather();
public Q_SLOTS:
    void loadForecast(KWeatherCore::WeatherForecast);
Q_SIGNALS:
    void reseted();

private:
    const KWeatherCore::HourlyWeatherForecast &getNthHour(int index) const;
    KWeatherCore::WeatherForecast m_location;
};

#endif // HOURLYNMODEL_H
