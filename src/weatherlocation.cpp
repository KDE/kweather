/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "weatherlocation.h"
#include "global.h"
#include "locationquerymodel.h"
#include "weatherdaymodel.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QQmlEngine>
#include <QTimeZone>
#include <utility>

WeatherLocation::WeatherLocation()
{
    m_weatherDayListModel = new WeatherDayListModel(this);
    m_weatherHourListModel = new WeatherHourListModel(this);
    m_lastUpdated = QDateTime::currentDateTime();
    this->m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &WeatherLocation::updateCurrentDateTime);
    this->m_timer->start(1000);
}

WeatherLocation::WeatherLocation(QString locationId, QString locationName, QString timeZone, float latitude, float longitude, SharedForecastPtr forecast)
    : m_forecast(forecast)
    , m_locationName(std::move(locationName))
    , m_locationId(std::move(locationId))
    , m_timeZone(std::move(timeZone))
    , m_latitude(latitude)
    , m_longitude(longitude)
{
    m_weatherDayListModel = new WeatherDayListModel(this);
    m_weatherHourListModel = new WeatherHourListModel(this);
    this->m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &WeatherLocation::updateCurrentDateTime);

    // prevent segfaults from js garbage collection
    QQmlEngine::setObjectOwnership(m_weatherDayListModel, QQmlEngine::CppOwnership);
    QQmlEngine::setObjectOwnership(m_weatherHourListModel, QQmlEngine::CppOwnership);

    if (forecast) {
        m_lastUpdated = forecast->createdTime();
        determineCurrentForecast();
    }
}

WeatherLocation *WeatherLocation::fromJson(const QJsonObject &obj)
{
    auto weatherLocation = new WeatherLocation(
        obj["locationId"].toString(), obj["locationName"].toString(), obj["timezone"].toString(), obj["latitude"].toDouble(), obj["longitude"].toDouble());
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

void WeatherLocation::updateData(QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> forecasts)
{
    m_forecast = forecasts;
    determineCurrentForecast();
    updateChart();
    m_lastUpdated = forecasts->createdTime();

    emit weatherRefresh(m_forecast);
    emit stopLoadingIndicator();
    writeToCache();

    emit propertyChanged();
}

void WeatherLocation::determineCurrentForecast()
{
    if (m_forecast->dailyWeatherForecast().empty())
        return;

    auto currentWeather = m_forecast->dailyWeatherForecast().begin()->hourlyWeatherForecast().begin();
    m_backgroundComponent = QStringLiteral("backgrounds/ClearDay.qml");

    bool isDayStyle = false; // make sure that if the background is definitively day, the colours match that

    if (currentWeather->weatherIcon() == QStringLiteral("weather-clear")) {
        m_backgroundComponent = QStringLiteral("backgrounds/ClearDay.qml");
        isDayStyle = true;

    } else if (currentWeather->weatherIcon() == QStringLiteral("weather-clear-night")) {
        m_backgroundComponent = QStringLiteral("backgrounds/ClearNight.qml");

    } else if (currentWeather->weatherIcon() == QStringLiteral("weather-clouds")) {
        m_backgroundComponent = QStringLiteral("backgrounds/CloudyDay.qml");
        isDayStyle = true;

    } else if (currentWeather->weatherIcon() == QStringLiteral("weather-clouds-night") || currentWeather->weatherIcon() == QStringLiteral("weather-overcast")) {
        m_backgroundComponent = QStringLiteral("backgrounds/CloudyNight.qml");

    } else if (currentWeather->weatherIcon() == QStringLiteral("weather-few-clouds")) {
        m_backgroundComponent = QStringLiteral("backgrounds/PartlyCloudyDay.qml");
        isDayStyle = true;

    } else if (currentWeather->weatherIcon() == QStringLiteral("weather-few-clouds-night")) {
        m_backgroundComponent = QStringLiteral("backgrounds/PartlyCloudyNight.qml");

    } else if (currentWeather->weatherIcon() == QStringLiteral("weather-fog") || currentWeather->weatherIcon() == QStringLiteral("weather-mist")) {
        m_backgroundComponent = QStringLiteral("backgrounds/Misty.qml");
        isDayStyle = true;

    } else if (currentWeather->weatherIcon() == QStringLiteral("weather-freezing-rain") || currentWeather->weatherIcon() == QStringLiteral("weather-snow-hail")
               || currentWeather->weatherIcon() == QStringLiteral("weather-showers") || currentWeather->weatherIcon() == QStringLiteral("weather-showers-day")
               || currentWeather->weatherIcon() == QStringLiteral("weather-showers-scattered")
               || currentWeather->weatherIcon() == QStringLiteral("weather-showers-scattered-day")
               || currentWeather->weatherIcon() == QStringLiteral("weather-storm") || currentWeather->weatherIcon() == QStringLiteral("weather-storm-day")) {
        m_backgroundComponent = QStringLiteral("backgrounds/RainyDay.qml");
        isDayStyle = true;

    } else if (currentWeather->weatherIcon() == QStringLiteral("weather-showers-night")
               || currentWeather->weatherIcon() == QStringLiteral("weather-showers-scattered-night")
               || currentWeather->weatherIcon() == QStringLiteral("weather-storm-night")) {
        m_backgroundComponent = QStringLiteral("backgrounds/RainyNight.qml");

    } else if (currentWeather->weatherIcon() == QStringLiteral("weather-hail") || currentWeather->weatherIcon() == QStringLiteral("weather-snow-scattered")
               || currentWeather->weatherIcon() == QStringLiteral("weather-snow")) {
        m_backgroundComponent = QStringLiteral("backgrounds/SnowyDay.qml");
        isDayStyle = true;

    } else if (currentWeather->weatherIcon() == QStringLiteral("weather-snow-scattered-night")) {
        m_backgroundComponent = QStringLiteral("backgrounds/SnowyNight.qml");
    }

    if (isDayStyle) {
        m_cardBackgroundColor = QStringLiteral("#fefefe");
        m_cardTextColor = QStringLiteral("black");
        m_backgroundColor = QStringLiteral("#3daee2");
        m_textColor = m_cardTextColor;
        m_iconColor = QStringLiteral("#4d4d4d");
    } else {
        m_cardBackgroundColor = QStringLiteral("#333333");
        m_cardTextColor = QStringLiteral("#eeeeee");
        m_backgroundColor = QStringLiteral("#222222");
        m_textColor = m_cardTextColor;
        m_iconColor = QStringLiteral("white");
    }
    emit currentForecastChange();
}

void WeatherLocation::initData(SharedForecastPtr fc)
{
    m_forecast = fc;
    determineCurrentForecast();
    emit weatherRefresh(m_forecast);
    emit propertyChanged();
}

void WeatherLocation::update()
{
    std::vector<KWeatherCore::Sunrise> sunriseVec;
    const auto &dayVec = m_forecast->dailyWeatherForecast();
    sunriseVec.reserve(dayVec.size());

    for (const auto &day : dayVec)
        sunriseVec.push_back(day.sunrise());

    auto pendingForecast = m_source.requestData(latitude(), longitude(), timeZone(), sunriseVec);
    connect(pendingForecast, &KWeatherCore::PendingWeatherForecast::finished, [this, pendingForecast] {
        this->updateData(pendingForecast->value());
        pendingForecast->deleteLater();
    });
}

void WeatherLocation::writeToCache() const
{
    QJsonDocument doc;
    doc.setObject(m_forecast->toJson());

    QFile file;
    QString url = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir dir(url.append(QString("/cache"))); // create cache location
    if (!dir.exists())
        dir.mkpath(".");
    // should be this path: /home/user/.cache/kweather/cache/1234567 for
    // location with locationID 1234567
    file.setFileName(dir.path() + "/" + this->locationId());
    file.open(QIODevice::WriteOnly);
    file.write(doc.toJson(QJsonDocument::Compact)); // write json
}

void WeatherLocation::updateChart()
{
    m_maxTempList.clear();
    m_xAxisList.clear();
    for (const auto &day : m_forecast->dailyWeatherForecast()) {
        m_maxTempList.append(day.maxTemp());
        m_xAxisList.append(QLocale::system().toString(day.date(), QStringLiteral("ddd")));
    }

    Q_EMIT chartListChanged();
}

const QVariantList &WeatherLocation::maxTempList()
{
    return m_maxTempList;
}
const QVariantList &WeatherLocation::xAxisList()
{
    return m_xAxisList;
}

void WeatherLocation::updateCurrentDateTime()
{
    Q_EMIT currentTimeChanged();
    Q_EMIT currentDateChanged();
}
