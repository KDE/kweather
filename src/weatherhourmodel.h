/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef WEATHERHOURMODEL_H
#define WEATHERHOURMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QSettings>

#include "abstractweatherforecast.h"
#include "weatherlocationmodel.h"
#include "kweathersettings.h"

class WeatherLocation;
class WeatherHour : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString windDirection READ windDirection NOTIFY propertyChanged)
    Q_PROPERTY(QString weatherDescription READ weatherDescription NOTIFY propertyChanged)
    Q_PROPERTY(QString weatherIcon READ weatherIcon NOTIFY propertyChanged)
    Q_PROPERTY(float precipitation READ precipitation NOTIFY propertyChanged)
    Q_PROPERTY(float fog READ fog NOTIFY propertyChanged)
    Q_PROPERTY(QString windSpeed READ windSpeed NOTIFY propertyChanged)
    Q_PROPERTY(QString temperature READ temperature NOTIFY propertyChanged)
    Q_PROPERTY(QString temperatureRounded READ temperatureRounded NOTIFY propertyChanged)
    Q_PROPERTY(float humidity READ humidity NOTIFY propertyChanged)
    Q_PROPERTY(float pressure READ pressure NOTIFY propertyChanged)
    Q_PROPERTY(QDateTime date READ date NOTIFY propertyChanged)

public:
    explicit WeatherHour();
    explicit WeatherHour(AbstractHourlyWeatherForecast &forecast);

    inline QString windDirection()
    {
        return windDirection_;
    }
    inline QString weatherDescription()
    {
        return weatherDescription_;
    }
    inline QString weatherIcon()
    {
        return weatherIcon_;
    }
    inline float precipitation() const
    {
        return precipitation_;
    }
    inline float fog() const
    {
        return qRound(fog_);
    }
    inline QString windSpeed() const
    {
        QSettings settings;
        if (KWeatherSettings().speedUnits() == "kph")
            return QString::number(windSpeed_, 'g', 1) + "km/h";
        else
            return QString::number(windSpeed_ * 0.62, 'g', 1) + "mph";
    }
    inline QString temperature() const
    {
        QSettings settings;
        if (KWeatherSettings().temperatureUnits() == "Fahrenheit") {
            return QString::number(temperature_ * 1.8 + 32, 'f', 1) + "°";
        } else {
            return QString::number(temperature_, 'f', 1) + "°";
        }
    }
    QString temperatureRounded() const
    {
        QSettings settings;
        if (KWeatherSettings().temperatureUnits() == "Fahrenheit") {
            return QString::number(qRound(temperature_ * 1.8 + 32)) + "°";
        } else {
            return QString::number(qRound(temperature_)) + "°";
        }
    }
    inline float humidity() const
    {
        return qRound(humidity_);
    }
    inline float pressure() const
    {
        return qRound(pressure_);
    }
    inline QDateTime date() const
    {
        return date_;
    }

    inline void setWindDirection(QString windDirection)
    {
        this->windDirection_ = std::move(windDirection);
    }
    inline void setWeatherDescription(QString weatherDescription)
    {
        this->weatherDescription_ = std::move(weatherDescription);
    }
    inline void setWeatherIcon(QString weatherIcon)
    {
        this->weatherIcon_ = std::move(weatherIcon);
    }
    inline void setPrecipitation(float precipitation)
    {
        this->precipitation_ = precipitation;
    }
    inline void setFog(float fog)
    {
        this->fog_ = fog;
    }
    inline void setWindSpeed(float windSpeed)
    {
        this->windSpeed_ = windSpeed;
    }
    inline void setTemperature(float temperature)
    {
        this->temperature_ = temperature;
    }
    inline void setHumidity(float humidity)
    {
        this->humidity_ = humidity;
    }
    inline void setPressure(float pressure)
    {
        this->pressure_ = pressure;
    }
    inline void setDate(QDateTime date)
    {
        this->date_ = date;
    }
signals:
    void propertyChanged();

private:
    QString windDirection_;
    QString weatherDescription_;
    QString weatherIcon_;
    float precipitation_;
    float fog_;
    float windSpeed_;
    float temperature_;
    float humidity_;
    float pressure_;

    QDateTime date_;
};

class WeatherHourListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit WeatherHourListModel(WeatherLocation* location = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Q_INVOKABLE WeatherHour *get(int index);

    Q_INVOKABLE void updateHourView(int index);
    Q_INVOKABLE void updateUi();
public slots:
    void refreshHoursFromForecasts(AbstractWeatherForecast& forecast);

private:
    QList<WeatherHour *> hoursList;
    QList<int> dayList;
    int day = 0;
};

#endif // WEATHERHOURMODEL_H
