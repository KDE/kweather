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
#include "kweathersettings.h"
#include <QDir>
#include <QTimer>
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
    this->m_timer->start(60 - QDateTime::currentDateTime().currentMSecsSinceEpoch() % 60);
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
    this->m_timer->start(60 - QDateTime::currentDateTime().currentMSecsSinceEpoch() % 60);

    // prevent segfaults from js garbage collection
    QQmlEngine::setObjectOwnership(m_weatherDayListModel, QQmlEngine::CppOwnership);
    QQmlEngine::setObjectOwnership(m_weatherHourListModel, QQmlEngine::CppOwnership);

    if (forecast) {
        m_lastUpdated = forecast->createdTime();
        determineCurrentForecast();
    }
}
WeatherLocation *WeatherLocation::load(const QString &groupName)
{
    auto config = KWeatherSettings().sharedConfig()->group(Kweather::WEATHER_LOCATIONS_CFG_GROUP).group(groupName);
    if (config.isValid()) {
        return new WeatherLocation(groupName, config.readEntry("locationName"), config.readEntry("timezone"), config.readEntry("latitude").toFloat(),
                            config.readEntry("longitude").toFloat());
    } else {
        return nullptr;
    }
}
void WeatherLocation::save()
{
    auto config = KWeatherSettings(this).sharedConfig()->group(Kweather::WEATHER_LOCATIONS_CFG_GROUP).group(locationId());
    config.writeEntry("locationName", locationName());
    config.writeEntry("latitude", latitude());
    config.writeEntry("longitude", longitude());
    config.writeEntry("timezone", m_timeZone);
    config.sync();
}
void WeatherLocation::updateData(QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> forecasts)
{
    m_forecast = forecasts;

    // first time the timezone is empty
    if (m_timeZone.isEmpty()) {
        m_timeZone = forecasts->timezone();
        save();
    }

    determineCurrentForecast();
    updateChart();
    m_lastUpdated = forecasts->createdTime();

    emit weatherRefresh(m_forecast);
    emit stopLoadingIndicator();

    emit propertyChanged();
    updateCurrentDateTime();
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

void WeatherLocation::update()
{
    auto pendingForecast = m_source.requestData(latitude(), longitude());
    if (pendingForecast->isFinished()) {
        updateData(pendingForecast->value());
        pendingForecast->deleteLater();
    } else
        connect(pendingForecast, &KWeatherCore::PendingWeatherForecast::finished, [this, pendingForecast] {
            this->updateData(pendingForecast->value());
            pendingForecast->deleteLater();
        });
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
    m_timer->setInterval(60000);
    Q_EMIT currentTimeChanged();
    Q_EMIT currentDateChanged();
}
