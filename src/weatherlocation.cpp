/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "weatherlocation.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QQmlEngine>
#include <QTimeZone>
#include <QTimer>

#include <utility>

#include "global.h"
#include "kweathersettings.h"
#include "locationquerymodel.h"

WeatherLocation::WeatherLocation(QString locationId,
                                 QString locationName,
                                 QString timeZone,
                                 float latitude,
                                 float longitude,
                                 KWeatherCore::WeatherForecast forecast)
    : m_forecast(forecast)
    , m_locationName(std::move(locationName))
    , m_locationId(std::move(locationId))
    , m_timeZone(std::move(timeZone))
    , m_latitude(latitude)
    , m_longitude(longitude)
{
    this->m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &WeatherLocation::updateCurrentDateTime);
    this->m_timer->start(60 - QDateTime::currentDateTime().currentMSecsSinceEpoch() % 60);

    m_lastUpdated = forecast.createdTime();
    Q_EMIT lastUpdatedChanged();

    update();

    determineCurrentForecast();

    connect(this, &WeatherLocation::selectedDayChanged, this, [this] {
        m_hourForecasts.clear();
        if (!m_forecast.dailyWeatherForecast().empty()) {
            const auto hourForecasts = m_forecast.dailyWeatherForecast()[m_selectedDay].hourlyWeatherForecast();
            for (const KWeatherCore::HourlyWeatherForecast &hour : hourForecasts) {
                m_hourForecasts << hour;
            }
        }
        Q_EMIT hourForecastsChanged();
    });
}

WeatherLocation::WeatherLocation()
{
}

WeatherLocation *WeatherLocation::load(const QString &groupName)
{
    auto config = KWeatherSettings::self()->config()->group(KWeather::WEATHER_LOCATIONS_CFG_GROUP).group(groupName);
    if (config.isValid()) {
        return new WeatherLocation(groupName,
                                   config.readEntry("locationName"),
                                   config.readEntry("timezone"),
                                   config.readEntry("latitude").toFloat(),
                                   config.readEntry("longitude").toFloat());
    } else {
        return nullptr;
    }
}

void WeatherLocation::save()
{
    auto config = KWeatherSettings::self()->config()->group(KWeather::WEATHER_LOCATIONS_CFG_GROUP).group(locationId());
    config.writeEntry("locationName", locationName());
    config.writeEntry("latitude", latitude());
    config.writeEntry("longitude", longitude());
    config.writeEntry("timezone", m_timeZone);
    config.sync();
}

void WeatherLocation::saveOrder(int index)
{
    auto config = KWeatherSettings::self()->config()->group(KWeather::WEATHER_LOCATIONS_CFG_GROUP).group(locationId());
    config.writeEntry("index", index);
    config.sync();
}

int WeatherLocation::index()
{
    auto config = KWeatherSettings::self()->config()->group(KWeather::WEATHER_LOCATIONS_CFG_GROUP).group(locationId());
    auto res = config.readEntry("index");
    if (res.isEmpty()) {
        return -1;
    } else {
        return res.toInt();
    }
}

void WeatherLocation::deleteConfig()
{
    auto config = KWeatherSettings::self()->config()->group(KWeather::WEATHER_LOCATIONS_CFG_GROUP);
    config.deleteGroup(locationId());
    config.sync();
}

const QString &WeatherLocation::locationId() const
{
    return m_locationId;
}

const QString &WeatherLocation::locationName() const
{
    return m_locationName;
}

const QString &WeatherLocation::timeZone() const
{
    return m_timeZone;
}

float WeatherLocation::latitude() const
{
    return m_latitude;
}

float WeatherLocation::longitude() const
{
    return m_longitude;
}

QString WeatherLocation::lastUpdatedFormatted() const
{
    return QLocale().toString(lastUpdated().time(), QLocale::ShortFormat).toLower();
}

const QDateTime &WeatherLocation::lastUpdated() const
{
    return m_lastUpdated;
}

QString WeatherLocation::currentTimeFormatted() const
{
    return QLocale().toString(currentDateTime().time(), QLocale::ShortFormat).toLower();
}

QString WeatherLocation::currentDateFormatted() const
{
    return QLocale().toString(currentDateTime().toLocalTime().date(), QLocale::LongFormat);
}

QDateTime WeatherLocation::currentDateTime() const
{
    return QDateTime::currentDateTime().toTimeZone(QTimeZone(m_timeZone.toUtf8()));
}

const QString &WeatherLocation::backgroundComponent() const
{
    return m_backgroundComponent;
}

const QColor &WeatherLocation::backgroundColor() const
{
    return m_backgroundColor;
}

const QColor &WeatherLocation::textColor() const
{
    return m_textColor;
}

const QColor &WeatherLocation::cardBackgroundColor() const
{
    return m_cardBackgroundColor;
}

const QColor &WeatherLocation::cardTextColor() const
{
    return m_cardTextColor;
}

const QColor &WeatherLocation::cardSecondaryTextColor() const
{
    return m_cardSecondaryTextColor;
}

const QColor &WeatherLocation::iconColor() const
{
    return m_iconColor;
}

bool WeatherLocation::darkTheme() const
{
    return m_isDarkTheme;
}

QList<KWeatherCore::DailyWeatherForecast> WeatherLocation::dayForecasts() const
{
    return m_dayForecasts;
}

QList<KWeatherCore::HourlyWeatherForecast> WeatherLocation::hourForecasts() const
{
    return m_hourForecasts;
}

int WeatherLocation::selectedDay() const
{
    return m_selectedDay;
}

void WeatherLocation::setSelectedDay(int selectedDay)
{
    if (selectedDay != m_selectedDay) {
        m_selectedDay = selectedDay;
        Q_EMIT selectedDayChanged();
    }
}

QVariant WeatherLocation::currentHourForecast()
{
    if (!m_forecast.dailyWeatherForecast().empty() && !m_forecast.dailyWeatherForecast()[0].hourlyWeatherForecast().empty()) {
        return QVariant::fromValue(m_forecast.dailyWeatherForecast().begin()->hourlyWeatherForecast()[0]);
    }
    return {};
}

void WeatherLocation::updateData(KWeatherCore::WeatherForecast forecasts)
{
    m_forecast = forecasts;

    // first time the timezone is empty
    if (m_timeZone.isEmpty()) {
        m_timeZone = forecasts.timezone();
        Q_EMIT timeZoneChanged();
        save();
    }

    determineCurrentForecast();
    m_lastUpdated = forecasts.createdTime();
    Q_EMIT lastUpdatedChanged();

    m_hourForecasts.clear();
    if (!m_forecast.dailyWeatherForecast().empty() && m_forecast.dailyWeatherForecast().size() > m_selectedDay) {
        const auto hourForecasts = m_forecast.dailyWeatherForecast()[m_selectedDay].hourlyWeatherForecast();
        for (const KWeatherCore::HourlyWeatherForecast &hour : hourForecasts) {
            m_hourForecasts << hour;
        }
    }
    Q_EMIT hourForecastsChanged();

    m_dayForecasts.clear();
    for (const KWeatherCore::DailyWeatherForecast &day : m_forecast.dailyWeatherForecast()) {
        m_dayForecasts << day;
    }
    Q_EMIT dayForecastsChanged();

    Q_EMIT weatherRefresh(m_forecast);
    Q_EMIT stopLoadingIndicator();

    updateCurrentDateTime();
}

void WeatherLocation::determineCurrentForecast()
{
    if (m_forecast.dailyWeatherForecast().empty())
        return;

    auto currentWeather = m_forecast.dailyWeatherForecast().begin()->hourlyWeatherForecast().begin();
    m_backgroundComponent = QStringLiteral("qrc:/qt/qml/org/kde/kweather/backgrounds/qml/backgrounds/ClearDay.qml");

    bool isDayStyle = false; // make sure that if the background is definitively day, the colours match that
    m_sun = false;
    m_rain = false;
    m_cloud = false;
    m_snow = false;
    m_star = false;
    if (currentWeather->weatherIcon() == QStringLiteral("weather-clear")) {
        m_backgroundComponent = QStringLiteral("qrc:/qt/qml/org/kde/kweather/backgrounds/qml/backgrounds/ClearDay.qml");
        isDayStyle = true;
        m_topColor = {255, 193, 7}; //"#ffc107"
        m_bottomColor = {255, 193, 7};
        m_sun = true;
    } else if (currentWeather->weatherIcon() == QStringLiteral("weather-clear-night")) {
        m_backgroundComponent = QStringLiteral("qrc:/qt/qml/org/kde/kweather/backgrounds/qml/backgrounds/ClearNight.qml");
        m_topColor = {69, 90, 100}; // "#455a64"
        m_bottomColor = {38, 50, 56}; // "#263238"
        m_star = true;
    } else if (currentWeather->weatherIcon() == QStringLiteral("weather-clouds")) {
        m_backgroundComponent = QStringLiteral("qrc:/qt/qml/org/kde/kweather/backgrounds/qml/backgrounds/CloudyDay.qml");
        isDayStyle = true;
        m_topColor = {0, 188, 212}; // #00bcd4
        m_bottomColor = {36, 163, 222}; // #24a3de
        m_cloudColor = {224, 247, 250}; // #e0f7fa
        m_cloud = true;
    } else if (currentWeather->weatherIcon() == QStringLiteral("weather-clouds-night") || currentWeather->weatherIcon() == QStringLiteral("weather-overcast")) {
        m_backgroundComponent = QStringLiteral("qrc:/qt/qml/org/kde/kweather/backgrounds/qml/backgrounds/CloudyNight.qml");
        m_topColor = {69, 90, 100}; // #455a64
        m_bottomColor = {38, 50, 56}; // #263238
        m_cloudColor = {176, 190, 197}; // #b0bec5
        m_star = true;
        m_cloud = true;
    } else if (currentWeather->weatherIcon() == QStringLiteral("weather-few-clouds")) {
        m_backgroundComponent = QStringLiteral("qrc:/qt/qml/org/kde/kweather/backgrounds/qml/backgrounds/PartlyCloudyDay.qml");
        isDayStyle = true;
        m_topColor = {0, 188, 212}; // #00bcd4
        m_bottomColor = {36, 163, 222}; // #24a3de
        m_cloudColor = {224, 247, 250}; // #e0f7fa
        m_cloud = true;
    } else if (currentWeather->weatherIcon() == QStringLiteral("weather-few-clouds-night")) {
        m_backgroundComponent = QStringLiteral("qrc:/qt/qml/org/kde/kweather/backgrounds/qml/backgrounds/PartlyCloudyNight.qml");
        m_topColor = {69, 90, 100}; // "#455a64"
        m_bottomColor = {38, 50, 56}; // "#263238"
        m_cloudColor = {176, 190, 197}; // #b0bec5
        m_star = true;
        m_cloud = true;
    } else if (currentWeather->weatherIcon() == QStringLiteral("weather-fog") || currentWeather->weatherIcon() == QStringLiteral("weather-mist")) {
        m_backgroundComponent = QStringLiteral("qrc:/qt/qml/org/kde/kweather/backgrounds/qml/backgrounds/Misty.qml");
        isDayStyle = true;
        m_topColor = {194, 197, 203}; // #c2c5cb
        m_bottomColor = {91, 96, 107}; // #5b606b
        m_cloudColor = {218, 224, 236}; // #dae0ec
        m_cloud = true;

    } else if (currentWeather->weatherIcon() == QStringLiteral("weather-freezing-rain") || currentWeather->weatherIcon() == QStringLiteral("weather-snow-hail")
               || currentWeather->weatherIcon() == QStringLiteral("weather-showers") || currentWeather->weatherIcon() == QStringLiteral("weather-showers-day")
               || currentWeather->weatherIcon() == QStringLiteral("weather-showers-scattered")
               || currentWeather->weatherIcon() == QStringLiteral("weather-showers-scattered-day")
               || currentWeather->weatherIcon() == QStringLiteral("weather-storm") || currentWeather->weatherIcon() == QStringLiteral("weather-storm-day")) {
        m_backgroundComponent = QStringLiteral("qrc:/qt/qml/org/kde/kweather/backgrounds/qml/backgrounds/RainyDay.qml");
        isDayStyle = true;
        m_topColor = {54, 156, 203}; // #369ccb
        m_bottomColor = {26, 131, 179}; // #1A83B3
        m_cloudColor = {255, 255, 255}; // white
        m_cloud = true;
        m_rain = true;
    } else if (currentWeather->weatherIcon() == QStringLiteral("weather-showers-night")
               || currentWeather->weatherIcon() == QStringLiteral("weather-showers-scattered-night")
               || currentWeather->weatherIcon() == QStringLiteral("weather-storm-night")) {
        m_backgroundComponent = QStringLiteral("qrc:/qt/qml/org/kde/kweather/backgrounds/qml/backgrounds/RainyNight.qml");
        m_backgroundComponent = QStringLiteral("qrc:/qt/qml/org/kde/kweather/backgrounds/qml/backgrounds/CloudyNight.qml");
        m_topColor = {69, 90, 100}; // #455a64
        m_bottomColor = {38, 50, 56}; // #263238
        m_cloudColor = {176, 190, 197}; // #b0bec5
        m_cloud = true;
        m_rain = true;
    } else if (currentWeather->weatherIcon() == QStringLiteral("weather-hail") || currentWeather->weatherIcon() == QStringLiteral("weather-snow-scattered")
               || currentWeather->weatherIcon() == QStringLiteral("weather-snow")) {
        m_backgroundComponent = QStringLiteral("qrc:/qt/qml/org/kde/kweather/backgrounds/qml/backgrounds/SnowyDay.qml");
        isDayStyle = true;
        m_topColor = {53, 217, 237}; // #35d9ed
        m_bottomColor = {36, 163, 222}; // #24a3de
        m_snow = true;
    } else if (currentWeather->weatherIcon() == QStringLiteral("weather-snow-scattered-night")) {
        m_backgroundComponent = QStringLiteral("qrc:/qt/qml/org/kde/kweather/backgrounds/qml/backgrounds/SnowyNight.qml");
        m_topColor = {69, 90, 100}; // #455a64
        m_bottomColor = {38, 50, 56}; // #263238
        m_snow = true;
    }

    if (isDayStyle) {
        m_cardBackgroundColor = QStringLiteral("#fefefe");
        m_cardTextColor = QStringLiteral("black");
        m_cardSecondaryTextColor = QStringLiteral("#404040");
        m_backgroundColor = QStringLiteral("#3daee2");
        m_textColor = m_cardTextColor;
        m_iconColor = QStringLiteral("#4d4d4d");
        m_isDarkTheme = false;
    } else {
        m_cardBackgroundColor = QStringLiteral("#333333");
        m_cardTextColor = QStringLiteral("#eeeeee");
        m_cardSecondaryTextColor = QStringLiteral("#C2C2C2");
        m_backgroundColor = QStringLiteral("#222222");
        m_textColor = m_cardTextColor;
        m_iconColor = QStringLiteral("white");
        m_isDarkTheme = true;
    }

    Q_EMIT currentForecastChanged();
}

void WeatherLocation::update()
{
    auto pendingForecast = m_source.requestData(latitude(), longitude());
    connect(pendingForecast, &KWeatherCore::PendingWeatherForecast::finished, this, [this, pendingForecast]() {
        this->updateData(pendingForecast->value());
        pendingForecast->deleteLater();
    });
}

void WeatherLocation::updateCurrentDateTime()
{
    m_timer->setInterval(60000);
    Q_EMIT currentTimeChanged();
    Q_EMIT currentDateChanged();
}

const QColor &WeatherLocation::topColor() const
{
    return m_topColor;
}
const QColor &WeatherLocation::bottomColor() const
{
    return m_bottomColor;
}
const QColor &WeatherLocation::cloudColor() const
{
    return m_cloudColor;
}
bool WeatherLocation::rain() const
{
    return m_rain;
}
bool WeatherLocation::cloud() const
{
    return m_cloud;
}
bool WeatherLocation::star() const
{
    return m_star;
}
bool WeatherLocation::sun() const
{
    return m_sun;
}
bool WeatherLocation::snow() const
{
    return m_snow;
}

#include "moc_weatherlocation.cpp"
