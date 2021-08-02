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
#include <QDateTime>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QTimeZone>
#include <QTimer>
#include <utility>


using SharedForecastPtr = QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast>;
class WeatherDayListModel;
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
    Q_PROPERTY(QString lastUpdated READ lastUpdatedFormatted NOTIFY propertyChanged)
    Q_PROPERTY(QString currentTime READ currentTimeFormatted NOTIFY currentTimeChanged)
    Q_PROPERTY(QString currentDate READ currentDateFormatted NOTIFY currentDateChanged)
    Q_PROPERTY(WeatherDayListModel *dayListModel READ weatherDayListModel NOTIFY propertyChanged)
    Q_PROPERTY(WeatherHourListModel *hourListModel READ weatherHourListModel NOTIFY propertyChanged)

    Q_PROPERTY(QString backgroundComponent READ backgroundComponent NOTIFY currentForecastChange)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor NOTIFY currentForecastChange)
    Q_PROPERTY(QColor textColor READ textColor NOTIFY currentForecastChange)
    Q_PROPERTY(QColor cardBackgroundColor READ cardBackgroundColor NOTIFY currentForecastChange)
    Q_PROPERTY(QColor cardTextColor READ cardTextColor NOTIFY currentForecastChange)
    Q_PROPERTY(QColor iconColor READ iconColor NOTIFY currentForecastChange)
    Q_PROPERTY(bool darkTheme READ darkTheme NOTIFY currentForecastChange)
    Q_PROPERTY(double maxTempLimit READ maxTempLimit NOTIFY currentForecastChange)
    Q_PROPERTY(double minTempLimit READ minTempLimit NOTIFY currentForecastChange)



public:
    WeatherLocation();
    explicit WeatherLocation(QString locationId,
                             QString locationName,
                             QString timeZone,
                             float latitude,
                             float longitude,
                             SharedForecastPtr forecast = SharedForecastPtr(new KWeatherCore::WeatherForecast));
    void save();
    static WeatherLocation *load(const QString &groupName);
    WeatherHour *currentWeather() const;
    Q_INVOKABLE void update();

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
        return currentDateTime().toString("hh:mm ap");
    }
    QString currentDateFormatted() const
    {
        return currentDateTime().toString("dd MMM yyyy");
    }
    QDateTime currentDateTime() const
    {
        return QDateTime::currentDateTime().toTimeZone(QTimeZone(m_timeZone.toUtf8()));
    }
    void setLastUpdated(QDateTime lastUpdated)
    {
        m_lastUpdated = std::move(lastUpdated);
        emit propertyChanged();
    }
    const QString &backgroundComponent() const
    {
        return m_backgroundComponent;
    };
    const QColor &backgroundColor() const
    {
        return m_backgroundColor;
    };
    const QColor &textColor() const
    {
        return m_textColor;
    };
    const QColor &cardBackgroundColor() const
    {
        return m_cardBackgroundColor;
    };
    const QColor &cardTextColor() const
    {
        return m_cardTextColor;
    };

    const QColor &iconColor() const
    {
        return m_iconColor;
    }

    double maxTempLimit() const
    {
        return m_maxTempLimit;
    }
    double minTempLimit() const
    {
        return m_minTempLimit;
    }
    bool darkTheme() const
    {
        return m_isDarkTheme;
    }
    Q_INVOKABLE void initSeries(QtCharts::QAbstractSeries *series);
    Q_INVOKABLE void initAxes(QObject *axisX, QObject *axisY);


    // for restore order of locations
    void saveOrder(int index);
    int index();
    void deleteConfig();
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
    void determineCurrentForecast();
    void updateSeries();
    void updateAxes();

    KWeatherCore::WeatherForecastSource m_source;
    SharedForecastPtr m_forecast;

    // chart related fields
    QtCharts::QSplineSeries *m_series = nullptr;
    QVector<QPointF> m_vector;
    double m_maxTempLimit {100};
    double m_minTempLimit {0};
    QtCharts::QDateTimeAxis *m_axisX {nullptr};
    QtCharts::QValueAxis *m_axisY {nullptr};



    // background related fields
    QColor m_backgroundColor;
    QColor m_textColor;
    QColor m_cardBackgroundColor;
    QColor m_cardTextColor;
    QColor m_iconColor;
    QString m_backgroundComponent = QStringLiteral("backgrounds/ClearDay.qml");
    bool m_isDarkTheme = false;

    QString m_locationName, m_locationId;
    QString m_timeZone;
    QDateTime m_lastUpdated;
    QTimer *m_timer;
    float m_latitude, m_longitude;

    WeatherDayListModel *m_weatherDayListModel = nullptr;
    WeatherHourListModel *m_weatherHourListModel = nullptr;
};
