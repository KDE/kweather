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
#include <QAbstractListModel>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <utility>

class WeatherDayListModel;
class WeatherHourListModel;
class AbstractWeatherAPI;
class AbstractWeatherForecast;
class LocationQueryResult;
class GeoIPLookup;
class GeoTimeZone;
class NMISunriseAPI;
class WeatherLocation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ locationName NOTIFY propertyChanged)
    Q_PROPERTY(QString lastUpdated READ lastUpdatedFormatted NOTIFY propertyChanged)
    Q_PROPERTY(WeatherDayListModel *dayListModel READ weatherDayListModel NOTIFY propertyChanged)
    Q_PROPERTY(WeatherHourListModel *hourListModel READ weatherHourListModel NOTIFY propertyChanged)
    Q_PROPERTY(AbstractHourlyWeatherForecast *currentWeather READ currentWeather NOTIFY currentForecastChange)

public:
    explicit WeatherLocation(AbstractWeatherForecast *forecast = nullptr);
    explicit WeatherLocation(AbstractWeatherAPI *weatherBackendProvider, QString locationId, QString locationName, QString timeZone, float latitude, float longitude, AbstractWeatherForecast *forecast = nullptr);
    ~WeatherLocation();
    static WeatherLocation *fromJson(const QJsonObject &json);
    QJsonObject toJson();
    void save();

    Q_INVOKABLE void updateBackend()
    {
        if (weatherBackendProvider() != nullptr)
            weatherBackendProvider()->update();
    }

    inline QString locationId()
    {
        return locationId_;
    }
    inline QString locationName()
    {
        return locationName_;
    }
    inline QString &timeZone()
    {
        return timeZone_;
    };
    inline float latitude()
    {
        return latitude_;
    }
    inline float longitude()
    {
        return longitude_;
    }
    inline AbstractHourlyWeatherForecast *currentWeather()
    {
        return currentWeather_;
    }
    inline WeatherDayListModel *weatherDayListModel()
    {
        return weatherDayListModel_;
    }
    inline WeatherHourListModel *weatherHourListModel()
    {
        return weatherHourListModel_;
    }
    inline AbstractWeatherForecast *forecast()
    {
        return forecast_;
    }
    inline AbstractWeatherAPI *weatherBackendProvider()
    {
        return weatherBackendProvider_;
    }
    inline QString lastUpdatedFormatted()
    {
        return lastUpdated().toString("hh:mm ap");
    }
    inline QDateTime lastUpdated()
    {
        return lastUpdated_;
    }
    inline void setLastUpdated(QDateTime lastUpdated)
    {
        this->lastUpdated_ = std::move(lastUpdated);
        emit propertyChanged();
    }
    void determineCurrentForecast();
    void initData(AbstractWeatherForecast *fc);
    void insertSunriseData();
    void update();

public slots:
    void updateData(AbstractWeatherForecast *fc);

signals:
    void weatherRefresh(AbstractWeatherForecast *fc); // sent when weather data is refreshed
    void currentForecastChange();
    void timeZoneSet();
    void propertyChanged(); // avoid warning
    void stopLoadingIndicator();

private:
    void writeToCache(AbstractWeatherForecast *fc);
    QJsonDocument convertToJson(AbstractWeatherForecast *fc);
    QString locationName_;
    QString timeZone_;
    QString locationId_;
    QDateTime lastUpdated_;
    float latitude_, longitude_;
    QList<AbstractSunrise *> sunriseList;
    WeatherDayListModel *weatherDayListModel_;
    WeatherHourListModel *weatherHourListModel_;

    AbstractWeatherAPI *weatherBackendProvider_;
    AbstractHourlyWeatherForecast *currentWeather_;
    AbstractWeatherForecast *forecast_;
    NMISunriseAPI *nmiSunriseApi_;
    GeoTimeZone *geoTimeZone_;
};

class WeatherLocationListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit WeatherLocationListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

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
public slots:
    void addLocation(LocationQueryResult *ret);

private:
    void addCurrentLocation();
    GeoIPLookup *geoPtr;
    QList<WeatherLocation *> locationsList;
};

#endif // WEATHERLOCATIONMODEL_H
