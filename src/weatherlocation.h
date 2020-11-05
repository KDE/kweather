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
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>

class WeatherDayListModel;
class WeatherHourListModel;
class WeatherHour;
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
    explicit WeatherLocation(QString locationId, QString locationName, QString timeZone, float latitude, float longitude, const KWeatherCore::WeatherForecast &forecast);
    static WeatherLocation *fromJson(const QJsonObject &json);
    QJsonObject toJson();
    void save();

    const QString &locationId() const;
    const QString &locationName() const;
    const QString &timeZone() const;
    float latitude() const;
    float longitude() const;
    WeatherHour *currentWeather() const;
    WeatherDayListModel *weatherDayListModel() const;
    WeatherHourListModel *weatherHourListModel() const;
    QString lastUpdatedFormatted() const;
    const QDateTime &lastUpdated() const;
    void setLastUpdated(const QDateTime &lastUpdated);
    void initData(QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> fc);
    void update();

    const QString &backgroundComponent() const;
    const QString &backgroundColor() const;
    const QString &textColor() const;
    const QString &cardBackgroundColor() const;
    const QString &cardTextColor() const;
    const QString &iconColor() const;
    double maxTempLimit() const;
    double minTempLimit() const;
    bool darkTheme() const;
    Q_INVOKABLE void initSeries(QtCharts::QAbstractSeries *series);
    Q_INVOKABLE void initAxes(QObject *axisX, QObject *axisY);

Q_SIGNALS:
    void currentForecastChange();
    void propertyChanged(); // avoid warning
    void stopLoadingIndicator();
    void weatherRefresh(QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> forecast);
private Q_SLOTS:
    void determineCurrentForecast();
    void determineCurrentBackgroundWeatherComponent();

private:
    void writeToCache(const KWeatherCore::WeatherForecast &fc);
    QJsonDocument convertToJson(const KWeatherCore::WeatherForecast &fc);

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
    QtCharts::QDateTimeAxis *m_axisX = nullptr;
    QtCharts::QValueAxis *m_axisY = nullptr;

    QString m_locationName, m_locationId;
    QString m_timeZone;
    QDateTime m_lastUpdated;
    float m_latitude, m_longitude;

    KWeatherCore::WeatherForecastSource m_weatherSource;

    WeatherDayListModel *m_weatherDayListModel = nullptr;
    WeatherHourListModel *m_weatherHourListModel = nullptr;

    WeatherHour *m_currentWeather = nullptr;

    void updateSeries();
    void updateAxes();
};
