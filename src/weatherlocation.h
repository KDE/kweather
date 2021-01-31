/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once
#include "weatherhourmodel.h"

#include <KWeatherCore/WeatherForecastSource>
#include <QAbstractListModel>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QDateTime>
#include <QTimeZone>
#include <QTimer>
#include <utility>
using SharedForecastPtr = QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast>;
class WeatherDayListModel;
class WeatherLocation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ locationName NOTIFY propertyChanged)
    Q_PROPERTY(QString backend READ backend NOTIFY propertyChanged)
    Q_PROPERTY(QString lastUpdated READ lastUpdatedFormatted NOTIFY propertyChanged)
    Q_PROPERTY(QString currentTime READ currentTimeFormatted NOTIFY currentTimeChanged)
    Q_PROPERTY(QString currentDate READ currentDateFormatted NOTIFY currentDateChanged)
    Q_PROPERTY(WeatherDayListModel *dayListModel READ weatherDayListModel NOTIFY propertyChanged)
    Q_PROPERTY(WeatherHourListModel *hourListModel READ weatherHourListModel NOTIFY propertyChanged)
    Q_PROPERTY(WeatherHour *currentWeather READ currentWeather NOTIFY currentForecastChange)

    Q_PROPERTY(QString backgroundComponent READ backgroundComponent NOTIFY currentForecastChange)
    Q_PROPERTY(QString backgroundColor READ backgroundColor NOTIFY currentForecastChange)
    Q_PROPERTY(QString textColor READ textColor NOTIFY currentForecastChange)
    Q_PROPERTY(QString cardBackgroundColor READ cardBackgroundColor NOTIFY currentForecastChange)
    Q_PROPERTY(QString cardTextColor READ cardTextColor NOTIFY currentForecastChange)
    Q_PROPERTY(QString iconColor READ iconColor NOTIFY currentForecastChange)

    Q_PROPERTY(QVariantList maxTempList READ maxTempList NOTIFY chartListChanged)
    Q_PROPERTY(QVariantList xAxisList READ xAxisList NOTIFY chartListChanged)
public:
    WeatherLocation();
    explicit WeatherLocation(QString locationId,
                             QString locationName,
                             QString timeZone,
                             float latitude,
                             float longitude,
                             SharedForecastPtr forecast = SharedForecastPtr());
    static WeatherLocation *fromJson(const QJsonObject &json);
    QJsonObject toJson();
    void save();

    const QString &locationId() const
    {
        return m_locationId;
    }
    const QString &locationName() const
    {
        return m_locationName;
    }
    const QString &timeZone() const
    {
        return m_timeZone;
    };
     float latitude() const
    {
        return m_latitude;
    }
     float longitude() const
    {
        return m_longitude;
    }
     WeatherHour currentWeather() const;
     WeatherDayListModel *weatherDayListModel() const
    {
        return m_weatherDayListModel;
    }
     WeatherHourListModel *weatherHourListModel() const
    {
        return m_weatherHourListModel;
    }
    QString lastUpdatedFormatted() const
    {
        return lastUpdated().toString("hh:mm ap");
    }
    const QDateTime &lastUpdated() const
    {
        return m_lastUpdated;
    }
    QString currentTimeFormatted() const
    {
        return currentTime().toString("hh:mm ap");
    }
    QTime currentTime() const
    {
        return QDateTime::currentDateTime().toTimeZone(QTimeZone(m_timeZone.toUtf8())).time();
    }
    QString currentDateFormatted() const
    {
        return currentDate().toString("dd MMM yyyy");
    }
    QDate currentDate() const
    {
        return QDateTime::currentDateTime().toTimeZone(QTimeZone(m_timeZone.toUtf8())).date();
    }
    void setLastUpdated(QDateTime lastUpdated)
    {
        m_lastUpdated = std::move(lastUpdated);
        emit propertyChanged();
    }
    void initData(SharedForecastPtr fc);
    void update();

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

    const QVariantList &maxTempList();
    const QVariantList &xAxisList();
public slots:
    void updateData(QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> forecasts);

signals:
    void weatherRefresh(QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> forecasts); // sent when weather data is refreshed
    void currentForecastChange();
    void propertyChanged(); // avoid warning
    void stopLoadingIndicator();
    void currentTimeChanged();
    void currentDateChanged();

    void chartListChanged();
private slots:
    void updateCurrentDateTime();
private:
    void writeToCache() const;
    void determineCurrentForecast();

    KWeatherCore::WeatherForecastSource m_source;
    SharedForecastPtr m_forecast;

    // chart related fields
    QVariantList m_maxTempList, m_xAxisList;

    // background related fields
    QString m_backgroundColor;
    QString m_textColor;
    QString m_cardBackgroundColor;
    QString m_cardTextColor;
    QString m_iconColor;
    QString m_backgroundComponent = QStringLiteral("backgrounds/ClearDay.qml");

    QString m_locationName, m_locationId;
    QString m_timeZone;
    QDateTime m_lastUpdated;
    QTimer *m_timer;
    float m_latitude, m_longitude;

    WeatherDayListModel *m_weatherDayListModel = nullptr;
    WeatherHourListModel *m_weatherHourListModel = nullptr;

    void updateChart();
};
