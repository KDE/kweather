/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef WEATHERDAYMODEL_H
#define WEATHERDAYMODEL_H
class WeatherLocation;

#include <QAbstractListModel>
#include <QObject>
#include <QSettings>

#include "abstractweatherforecast.h"
#include "weatherlocationmodel.h"
#include "abstractsunrise.h"

// generated from the forecasts list
class WeatherDay : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString weatherDescription READ weatherDescription NOTIFY propertyChanged)
    Q_PROPERTY(QString weatherIcon READ weatherIcon NOTIFY propertyChanged)
    Q_PROPERTY(QDateTime date READ date NOTIFY propertyChanged)
    Q_PROPERTY(QString maxTemp READ maxTemp NOTIFY propertyChanged)
    Q_PROPERTY(QString minTemp READ minTemp NOTIFY propertyChanged)
    Q_PROPERTY(float precipitation READ precipitation NOTIFY propertyChanged)
    Q_PROPERTY(float uvIndex READ uvIndex NOTIFY propertyChanged)
    Q_PROPERTY(float humidity READ humidity NOTIFY propertyChanged)
    Q_PROPERTY(float pressure READ pressure NOTIFY propertyChanged)

    Q_PROPERTY(QString sunrise READ sunrise NOTIFY propertyChanged)
    Q_PROPERTY(QString sunset READ sunset NOTIFY propertyChanged)
    Q_PROPERTY(QString moonPhase READ moonPhase NOTIFY propertyChanged)

public:
    explicit WeatherDay();
    // list of forecasts on this day
    explicit WeatherDay(AbstractDailyWeatherForecast& dailyForecast, AbstractSunrise& sunrise);

    inline QString weatherDescription() {return weatherDescription_;}
    inline QString weatherIcon() {return weatherIcon_;}
    inline QDateTime date() {return QDateTime(date_);}
    inline QString maxTemp() {
        QSettings settings;
        if (settings.value("Global/temperatureUnits", "Celsius").toString() == "Fahrenheit") {
            return QString::number(qRound(maxTemp_ * 1.8 + 32)) + "°";
        } else {
            return QString::number(qRound(maxTemp_)) + "°";
        }
    }
    inline QString minTemp() {
        QSettings settings;
        if (settings.value("Global/temperatureUnits", "Celsius").toString() == "Fahrenheit") {
            return QString::number(qRound(minTemp_ * 1.8 + 32)) + "°";
        } else {
            return QString::number(qRound(minTemp_)) + "°";
        }
    }
    inline float precipitation() {return precipitation_;}
    inline float uvIndex() {return uvIndex_;}
    inline float humidity() {return humidity_;}
    inline float pressure() {return pressure_;}
    inline QString sunrise() {return sunrise_;}
    inline QString sunset() {return sunset_;}
    inline QString moonPhase() {return moonPhase_;}

signals:
    void propertyChanged();

private:
    QString weatherDescription_;
    QString weatherIcon_;
    QDate date_;
    float maxTemp_;
    float minTemp_;
    float precipitation_;
    float uvIndex_;
    float humidity_;
    float pressure_;

    QString sunrise_;
    QString sunset_;
    QString moonPhase_;
};

// caches WeatherDays, until signal is called to update and regenerate
class WeatherDayListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit WeatherDayListModel(WeatherLocation* location = nullptr);

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    Q_INVOKABLE WeatherDay* get(int index);

    Q_INVOKABLE void updateUi();

public slots:
    void refreshDaysFromForecasts(AbstractWeatherForecast& forecast);
    
private:
    QList<WeatherDay*> daysList;
};

#endif // WEATHERDAYMODEL_H
