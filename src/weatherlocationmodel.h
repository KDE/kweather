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

class WeatherDayListModel;
class WeatherHourListModel;
class WeatherHour;
class AbstractWeatherAPI;
class AbstractWeatherForecast;
class LocationQueryResult;
class GeoIPLookup;
class GeoTimeZone;
class NMISunriseAPI;
class WeatherLocation : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.kweather.WeatherLocation")
    Q_PROPERTY(QString name READ locationName NOTIFY propertyChanged)
    Q_PROPERTY(QString backend READ backend NOTIFY propertyChanged)
    Q_PROPERTY(QString lastUpdated READ lastUpdatedFormatted NOTIFY propertyChanged)
    Q_PROPERTY(WeatherDayListModel *dayListModel READ weatherDayListModel NOTIFY propertyChanged)
    Q_PROPERTY(WeatherHourListModel *hourListModel READ weatherHourListModel NOTIFY propertyChanged)
    Q_PROPERTY(WeatherHour *currentWeather READ currentWeather NOTIFY currentForecastChange)

public:
    WeatherLocation();
    explicit WeatherLocation(AbstractWeatherAPI *weatherBackendProvider,
                             QString locationId,
                             QString locationName,
                             QString timeZone,
                             float latitude,
                             float longitude,
                             Kweather::Backend backend = Kweather::Backend::NMI,
                             AbstractWeatherForecast forecast = AbstractWeatherForecast());
    ~WeatherLocation();
    static WeatherLocation *fromJson(const QJsonObject &json);
    QJsonObject toJson();
    void save();
    Q_SCRIPTABLE QString getWeatherData()
    {
        return QJsonDocument(this->forecast().toJson()).toJson();
    }
    Q_INVOKABLE void updateBackend()
    {
        if (weatherBackendProvider() != nullptr)
            weatherBackendProvider()->update();
    }

    Q_SCRIPTABLE inline QString locationId()
    {
        return locationId_;
    }
    Q_SCRIPTABLE inline QString locationName()
    {
        return locationName_;
    }
    Q_SCRIPTABLE inline QString &timeZone()
    {
        return timeZone_;
    };
    Q_SCRIPTABLE inline double latitude()
    {
        return latitude_;
    }
    Q_SCRIPTABLE inline double longitude()
    {
        return longitude_;
    }
    inline WeatherHour *currentWeather()
    {
        //        return currentWeather_ == nullptr ? new WeatherHour() : currentWeather_;
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
    inline AbstractWeatherForecast forecast()
    {
        return forecast_;
    }
    inline AbstractWeatherAPI *weatherBackendProvider()
    {
        return weatherBackendProvider_;
    }
    Q_SCRIPTABLE inline QString lastUpdatedFormatted()
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
    void initData(AbstractWeatherForecast fc);
    void update();
    void changeBackend(Kweather::Backend backend); // change backend on the fly
    inline QString backend()
    {
        switch (backend_) {
        case Kweather::Backend::NMI:
            return Kweather::API_NMI;
        case Kweather::Backend::OWM:
            return Kweather::API_OWM;
        default:
            return {};
        }
    };
public slots:
    void updateData(AbstractWeatherForecast &fc);

signals:
    void weatherRefresh(AbstractWeatherForecast &fc); // sent when weather data is refreshed
    Q_SCRIPTABLE void currentForecastChange();
    Q_SCRIPTABLE void propertyChanged(); // avoid warning
    void stopLoadingIndicator();

private:
    Kweather::Backend backend_ = Kweather::Backend::NMI;

    void writeToCache(AbstractWeatherForecast &fc);
    QJsonDocument convertToJson(AbstractWeatherForecast &fc);

    QString locationName_, locationId_;
    QString timeZone_;
    QDateTime lastUpdated_;
    float latitude_, longitude_;

    WeatherDayListModel *weatherDayListModel_ = nullptr;
    WeatherHourListModel *weatherHourListModel_ = nullptr;

    AbstractWeatherForecast forecast_;
    WeatherHour *currentWeather_ = nullptr;

    AbstractWeatherAPI *weatherBackendProvider_ = nullptr;
};

class WeatherLocationListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.kweather.LocationModel")
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

#ifndef Q_OS_ANRDOID
    Q_SCRIPTABLE void removed(QString locationID);
    Q_SCRIPTABLE void added(QString locationID);
#endif

private:
    void addCurrentLocation();
    GeoIPLookup *geoPtr = nullptr;
    QList<WeatherLocation *> locationsList;
};

#endif // WEATHERLOCATIONMODEL_H
