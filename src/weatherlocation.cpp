/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "weatherlocation.h"
#include "weatherdaymodel.h"
#include "weatherhourmodel.h"

#include <QAbstractSeries>
#include <QDateTimeAxis>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QQmlEngine>
#include <QSplineSeries>
#include <QTimeZone>
#include <QValueAxis>
#include <utility>

WeatherLocation::WeatherLocation()
{
    this->m_weatherDayListModel = new WeatherDayListModel(this);
    this->m_weatherHourListModel = new WeatherHourListModel(this);
    this->m_lastUpdated = QDateTime::currentDateTime();
    connect(this, &WeatherLocation::propertyChanged, this, &WeatherLocation::determineCurrentBackgroundWeatherComponent);
    connect(this, &WeatherLocation::propertyChanged, this, &WeatherLocation::determineCurrentForecast);
}

WeatherLocation::WeatherLocation(QString locationId, QString locationName, QString timeZone, float latitude, float longitude, const KWeatherCore::WeatherForecast &forecast)
    : m_locationName(std::move(locationName))
    , m_timeZone(std::move(timeZone))
    , m_latitude(latitude)
    , m_longitude(longitude)
    , m_locationId(std::move(locationId))
{
    this->m_weatherDayListModel = new WeatherDayListModel(this);
    this->m_weatherHourListModel = new WeatherHourListModel(this);
    this->m_lastUpdated = forecast.createdTime();

    // prevent segfaults from js garbage collection
    QQmlEngine::setObjectOwnership(this->m_weatherDayListModel, QQmlEngine::CppOwnership);
    QQmlEngine::setObjectOwnership(this->m_weatherHourListModel, QQmlEngine::CppOwnership);

    connect(this, &WeatherLocation::propertyChanged, this, &WeatherLocation::determineCurrentBackgroundWeatherComponent);
    connect(this, &WeatherLocation::propertyChanged, this, &WeatherLocation::determineCurrentForecast);
}

WeatherLocation *WeatherLocation::fromJson(const QJsonObject &obj)
{
    auto weatherLocation = new WeatherLocation(obj["locationId"].toString(), obj["locationName"].toString(), obj["timezone"].toString(), obj["latitude"].toDouble(), obj["longitude"].toDouble(), KWeatherCore::WeatherForecast());
    return weatherLocation;
}

QJsonObject WeatherLocation::toJson()
{
    QJsonObject obj;
    obj["locationId"] = locationId();
    obj["locationName"] = locationName();
    obj["latitude"] = latitude();
    obj["longitude"] = longitude();
    obj["timezone"] = m_timeZone;
    return obj;
}

void WeatherLocation::determineCurrentForecast()
{
    m_currentWeather->deleteLater();

    if (m_weatherHourListModel->rowCount({}) == 0) {
        m_currentWeather = new WeatherHour();
    } else {
        m_currentWeather = m_weatherHourListModel->currentForecast();
    }
    QQmlEngine::setObjectOwnership(m_currentWeather, QQmlEngine::CppOwnership); // prevent segfaults from js garbage collecting

    determineCurrentBackgroundWeatherComponent();
    updateSeries();
    emit currentForecastChange();
}

void WeatherLocation::determineCurrentBackgroundWeatherComponent()
{
    m_backgroundComponent = QStringLiteral("backgrounds/ClearDay.qml");

    bool isDayStyle = false; // make sure that if the background is definitively day, the colours match that

    if (m_currentWeather->weatherIcon() == QStringLiteral("weather-clear")) {
        m_backgroundComponent = QStringLiteral("backgrounds/ClearDay.qml");
        isDayStyle = true;

    } else if (m_currentWeather->weatherIcon() == QStringLiteral("weather-clear-night")) {
        m_backgroundComponent = QStringLiteral("backgrounds/ClearNight.qml");

    } else if (m_currentWeather->weatherIcon() == QStringLiteral("weather-clouds")) {
        m_backgroundComponent = QStringLiteral("backgrounds/CloudyDay.qml");
        isDayStyle = true;

    } else if (m_currentWeather->weatherIcon() == QStringLiteral("weather-clouds-night") || m_currentWeather->weatherIcon() == QStringLiteral("weather-overcast")) {
        m_backgroundComponent = QStringLiteral("backgrounds/CloudyNight.qml");

    } else if (m_currentWeather->weatherIcon() == QStringLiteral("weather-few-clouds")) {
        m_backgroundComponent = QStringLiteral("backgrounds/PartlyCloudyDay.qml");
        isDayStyle = true;

    } else if (m_currentWeather->weatherIcon() == QStringLiteral("weather-few-clouds-night")) {
        m_backgroundComponent = QStringLiteral("backgrounds/PartlyCloudyNight.qml");

    } else if (m_currentWeather->weatherIcon() == QStringLiteral("weather-fog") || m_currentWeather->weatherIcon() == QStringLiteral("weather-mist")) {
        m_backgroundComponent = QStringLiteral("backgrounds/Misty.qml");
        isDayStyle = true;

    } else if (m_currentWeather->weatherIcon() == QStringLiteral("weather-freezing-rain") || m_currentWeather->weatherIcon() == QStringLiteral("weather-snow-hail") || m_currentWeather->weatherIcon() == QStringLiteral("weather-showers") ||
               m_currentWeather->weatherIcon() == QStringLiteral("weather-showers-day") || m_currentWeather->weatherIcon() == QStringLiteral("weather-showers-scattered") ||
               m_currentWeather->weatherIcon() == QStringLiteral("weather-showers-scattered-day") || m_currentWeather->weatherIcon() == QStringLiteral("weather-storm") ||
               m_currentWeather->weatherIcon() == QStringLiteral("weather-storm-day")) {
        m_backgroundComponent = QStringLiteral("backgrounds/RainyDay.qml");
        isDayStyle = true;

    } else if (m_currentWeather->weatherIcon() == QStringLiteral("weather-showers-night") || m_currentWeather->weatherIcon() == QStringLiteral("weather-showers-scattered-night") ||
               m_currentWeather->weatherIcon() == QStringLiteral("weather-storm-night")) {
        m_backgroundComponent = QStringLiteral("backgrounds/RainyNight.qml");

    } else if (m_currentWeather->weatherIcon() == QStringLiteral("weather-hail") || m_currentWeather->weatherIcon() == QStringLiteral("weather-snow-scattered") || m_currentWeather->weatherIcon() == QStringLiteral("weather-snow")) {
        m_backgroundComponent = QStringLiteral("backgrounds/SnowyDay.qml");
        isDayStyle = true;

    } else if (m_currentWeather->weatherIcon() == QStringLiteral("weather-snow-scattered-night")) {
        m_backgroundComponent = QStringLiteral("backgrounds/SnowyNight.qml");
    }

    if (isDayStyle) {
        m_cardBackgroundColor = QStringLiteral("#fefefe");
        m_cardTextColor = QStringLiteral("black");
        m_backgroundColor = QStringLiteral("#3daee2");
        m_textColor = m_cardTextColor;
        m_iconColor = QStringLiteral("eff0f1");
        m_isDarkTheme = false;
    } else {
        m_cardBackgroundColor = QStringLiteral("#333333");
        m_cardTextColor = QStringLiteral("#eeeeee");
        m_backgroundColor = QStringLiteral("#222222");
        m_textColor = m_cardTextColor;
        m_iconColor = QStringLiteral("white");
        m_isDarkTheme = true;
    }
}

void WeatherLocation::initData(QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> fc)
{
    Q_EMIT weatherRefresh(fc);
    Q_EMIT propertyChanged();
}

void WeatherLocation::update()
{
    m_weatherSource.requestData(latitude(), longitude(), timeZone(), weatherDayListModel()->sunrise());
}

void WeatherLocation::writeToCache(const KWeatherCore::WeatherForecast &fc)
{
    QFile file;
    QString url = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir dir(url.append(QString("/cache"))); // create cache location
    if (!dir.exists())
        dir.mkpath(".");
    // should be this path: /home/user/.cache/kweather/cache/1234567 for
    // location with locationID 1234567
    file.setFileName(dir.path() + "/" + this->locationId());
    file.open(QIODevice::WriteOnly);
    file.write(convertToJson(fc).toJson(QJsonDocument::Compact)); // write json
    file.close();
}
QJsonDocument WeatherLocation::convertToJson(const KWeatherCore::WeatherForecast &fc)
{
    QJsonDocument doc;
    // doc.setObject(fc);
    return doc;
}

void WeatherLocation::initSeries(QtCharts::QAbstractSeries *series)
{
    if (series) {
        m_series = static_cast<QtCharts::QSplineSeries *>(series);
        updateSeries();
    }
}

void WeatherLocation::updateSeries()
{
    if (m_series && weatherDayListModel()->days().size() > 0) {
        m_vector.clear();
        int i = 0;
        double minTemp = std::numeric_limits<double>::max(), maxTemp = std::numeric_limits<double>::min();
        for (auto d : weatherDayListModel()->days()) {
            m_vector.append(QPointF(d->date().startOfDay().toSecsSinceEpoch(), d->maxTempNum()));
            minTemp = std::min<double>(d->minTempNum(), minTemp);
            maxTemp = std::max<double>(d->maxTempNum(), maxTemp);
            ++i;
        }

        // make enough room for the curve
        m_maxTempLimit = maxTemp + 5;
        m_minTempLimit = minTemp - 5;

        m_series->replace(m_vector);
        if (m_axisX) {
            m_axisX->setRange(weatherDayListModel()->days().front()->date().startOfDay(), weatherDayListModel()->days().back()->date().startOfDay());
        }
    }
}

void WeatherLocation::initAxes(QObject *axisX, QObject *axisY)
{
    using namespace QtCharts;
    if (axisX && axisY) {
        m_axisX = static_cast<QDateTimeAxis *>(axisX);
        m_axisY = static_cast<QValueAxis *>(axisY);
        updateAxes();
    }
}

// TODO: actually update the label
void WeatherLocation::updateAxes()
{
}
