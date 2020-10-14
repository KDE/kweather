/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

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
namespace QtCharts
{
class QAbstractSeries;
class QSplineSeries;
class QDateTimeAxis;
class QValueAxis;
}
class WeatherLocation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ locationName NOTIFY propertyChanged)
    Q_PROPERTY(QString backend READ backend NOTIFY propertyChanged)
    Q_PROPERTY(QString lastUpdated READ lastUpdatedFormatted NOTIFY propertyChanged)
    Q_PROPERTY(WeatherDayListModel *dayListModel READ weatherDayListModel NOTIFY propertyChanged)
    Q_PROPERTY(WeatherHourListModel *hourListModel READ weatherHourListModel NOTIFY propertyChanged)
    Q_PROPERTY(WeatherHour *currentWeather READ currentWeather NOTIFY currentForecastChange)

    Q_PROPERTY(QString backgroundComponent READ backgroundComponent NOTIFY currentForecastChange)
    Q_PROPERTY(QString backgroundColor READ backgroundColor NOTIFY currentForecastChange)
    Q_PROPERTY(QString textColor READ textColor NOTIFY currentForecastChange)
    Q_PROPERTY(QString cardBackgroundColor READ cardBackgroundColor NOTIFY currentForecastChange)
    Q_PROPERTY(QString cardTextColor READ cardTextColor NOTIFY currentForecastChange)
    Q_PROPERTY(QString iconColor READ iconColor NOTIFY currentForecastChange)
    Q_PROPERTY(bool darkTheme READ darkTheme NOTIFY currentForecastChange)

    Q_PROPERTY(double maxTempLimit READ maxTempLimit NOTIFY currentForecastChange)
    Q_PROPERTY(double minTempLimit READ minTempLimit NOTIFY currentForecastChange)

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
    void determineCurrentBackgroundWeatherComponent();
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

    const QString &backgroundComponent() const
    {
        return m_backgroundComponent;
    };
    const QString &backgroundColor() const
    {
        return m_backgroundColor;
    };
    const QString &textColor() const
    {
        return m_textColor;
    };
    const QString &cardBackgroundColor() const
    {
        return m_cardBackgroundColor;
    };
    const QString &cardTextColor() const
    {
        return m_cardTextColor;
    };
    const QString &iconColor() const
    {
        return m_iconColor;
    }
    const double maxTempLimit() const
    {
        return m_maxTempLimit;
    }
    const double minTempLimit() const
    {
        return m_minTempLimit;
    }

    bool darkTheme() const
    {
        return m_isDarkTheme;
    }
    Q_INVOKABLE void initSeries(QtCharts::QAbstractSeries *series);
    Q_INVOKABLE void initAxes(QObject *axisX, QObject *axisY);
public slots:
    void updateData(AbstractWeatherForecast &fc);

signals:
    void weatherRefresh(AbstractWeatherForecast &fc); // sent when weather data is refreshed
    void currentForecastChange();
    void propertyChanged(); // avoid warning
    void stopLoadingIndicator();

private:
    Kweather::Backend backend_ = Kweather::Backend::NMI;

    void writeToCache(AbstractWeatherForecast &fc);
    QJsonDocument convertToJson(AbstractWeatherForecast &fc);

    // background related fields
    QString m_backgroundColor;
    QString m_textColor;
    QString m_cardBackgroundColor;
    QString m_cardTextColor;
    QString m_iconColor;
    QString m_backgroundComponent = QStringLiteral("backgrounds/ClearDay.qml");

    // the QXYSeries from qml, for temperature chart
    QtCharts::QSplineSeries *m_series = nullptr;
    QVector<QPointF> m_vector;
    double m_maxTempLimit;
    double m_minTempLimit;
    bool m_isDarkTheme = false;

    // QtChart Axes
    QtCharts::QDateTimeAxis *m_axisX {nullptr};
    QtCharts::QValueAxis *m_axisY {nullptr};

    QString locationName_, locationId_;
    QString timeZone_;
    QDateTime lastUpdated_;
    float latitude_, longitude_;

    WeatherDayListModel *weatherDayListModel_ = nullptr;
    WeatherHourListModel *weatherHourListModel_ = nullptr;

    AbstractWeatherForecast forecast_;
    WeatherHour *currentWeather_ = nullptr;

    AbstractWeatherAPI *weatherBackendProvider_ = nullptr;

    void updateSeries();
    void updateAxes();
};
