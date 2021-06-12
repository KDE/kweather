/*
    SPDX-FileCopyrightText: 2021 HanYoung <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/
#ifndef HOURLYMODEL_H
#define HOURLYMODEL_H

#include <QAbstractListModel>
#include <KWeatherCore/WeatherForecast>
class LocationModel;
class HourlyModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString location READ location NOTIFY reseted)
    Q_PROPERTY(QString currentIcon READ currentIcon NOTIFY reseted)
    Q_PROPERTY(QString currentTemperature READ currentTemperature NOTIFY reseted)
    Q_PROPERTY(QString currentDescription READ currentDescription NOTIFY reseted)
public:
    enum HourlyRole {Time = Qt::UserRole + 1, Icon, Description, Temperature, Precipitation};
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual int rowCount(const QModelIndex &index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
    void loadForecast(QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast>, int);
    const QString &location() const;
    const QString &currentIcon() const;
    QString currentTemperature() const;
    const QString &currentDescription() const;
Q_SIGNALS:
    void reseted();
protected:
    friend class KWeather_1x4;
    void setLocationModel(LocationModel *locationModel) {
        m_locationModel = locationModel;
    }
private:
    const KWeatherCore::HourlyWeatherForecast &getNthHour(int index) const;
    QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> m_location;
    LocationModel *m_locationModel;
    int m_index = 0;
};

#endif // HOURLYNMODEL_H
