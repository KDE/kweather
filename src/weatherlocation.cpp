/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "weatherlocation.h"
#include "abstractweatherapi.h"
#include "abstractweatherforecast.h"
#include "geoiplookup.h"
#include "geotimezone.h"
#include "global.h"
#include "locationquerymodel.h"
#include "nmisunriseapi.h"
#include "nmiweatherapi2.h"
#include "owmweatherapi.h"
#include "weatherdaymodel.h"

#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QQmlEngine>
#include <QTimeZone>
#include <utility>

WeatherLocation::WeatherLocation()
{
    this->weatherDayListModel_ = new WeatherDayListModel(this);
    this->weatherHourListModel_ = new WeatherHourListModel(this);
    this->lastUpdated_ = QDateTime::currentDateTime();
    this->m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &WeatherLocation::updateCurrentDateTime);
    this->m_timer->start(1000);
}

WeatherLocation::WeatherLocation(AbstractWeatherAPI *weatherBackendProvider, QString locationId, QString locationName, QString timeZone, float latitude, float longitude, Kweather::Backend backend, AbstractWeatherForecast forecast)
    : backend_(backend)
    , locationName_(std::move(locationName))
    , timeZone_(std::move(timeZone))
    , latitude_(latitude)
    , longitude_(longitude)
    , locationId_(std::move(locationId))
    , forecast_(forecast)
    , weatherBackendProvider_(weatherBackendProvider)
{
    this->weatherDayListModel_ = new WeatherDayListModel(this);
    this->weatherHourListModel_ = new WeatherHourListModel(this);
    this->lastUpdated_ = forecast.timeCreated();
    this->m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &WeatherLocation::updateCurrentDateTime);

    // prevent segfaults from js garbage collection
    QQmlEngine::setObjectOwnership(this->weatherDayListModel_, QQmlEngine::CppOwnership);
    QQmlEngine::setObjectOwnership(this->weatherHourListModel_, QQmlEngine::CppOwnership);

    determineCurrentForecast();

    connect(this->weatherBackendProvider(), &AbstractWeatherAPI::updated, this, &WeatherLocation::updateData, Qt::UniqueConnection);
    this->m_timer->start(1000);
}

WeatherLocation *WeatherLocation::fromJson(const QJsonObject &obj)
{
    AbstractWeatherAPI *api; // don't fetch sunrise information, since it will be loaded from cache
    Kweather::Backend backendEnum;
    if (obj["backend"].toInt() == 0) {
        api = new NMIWeatherAPI2(obj["locationId"].toString(), obj["timezone"].toString(), obj["latitude"].toDouble(), obj["longitude"].toDouble());
        backendEnum = Kweather::Backend::NMI;
    } else {
        api = new OWMWeatherAPI(obj["locationId"].toString(), obj["timezone"].toString(), obj["latitude"].toDouble(), obj["longitude"].toDouble());
        backendEnum = Kweather::Backend::OWM;
    }
    auto weatherLocation = new WeatherLocation(api, obj["locationId"].toString(), obj["locationName"].toString(), obj["timezone"].toString(), obj["latitude"].toDouble(), obj["longitude"].toDouble(), backendEnum, AbstractWeatherForecast());
    return weatherLocation;
}

QJsonObject WeatherLocation::toJson()
{
    QJsonObject obj;
    obj["locationId"] = locationId();
    obj["locationName"] = locationName();
    obj["latitude"] = latitude();
    obj["longitude"] = longitude();
    obj["timezone"] = timeZone_;
    obj["backend"] = static_cast<int>(backend_);
    return obj;
}

void WeatherLocation::updateData(AbstractWeatherForecast &fc)
{
    forecast_ = fc;
    determineCurrentForecast();
    updateChart();
    lastUpdated_ = fc.timeCreated();

    emit weatherRefresh(forecast_);
    emit stopLoadingIndicator();
    writeToCache(forecast_);

    emit propertyChanged();
}

void WeatherLocation::determineCurrentForecast()
{
    delete currentWeather_;

    if (forecast().hourlyForecasts().count() == 0) {
        currentWeather_ = new WeatherHour();
    } else {
        long long minSecs = -1;
        QDateTime current = QDateTime::currentDateTime();

        // get closest forecast to current time
        for (auto forecast : forecast_.hourlyForecasts()) {
            if (minSecs == -1 || minSecs > llabs(forecast.date().secsTo(current))) {
                currentWeather_ = new WeatherHour(forecast);
                minSecs = llabs(forecast.date().secsTo(current));
            }
        }
    }
    QQmlEngine::setObjectOwnership(currentWeather_, QQmlEngine::CppOwnership); // prevent segfaults from js garbage collecting

    determineCurrentBackgroundWeatherComponent();
    emit currentForecastChange();
}

void WeatherLocation::determineCurrentBackgroundWeatherComponent()
{
    m_backgroundComponent = QStringLiteral("backgrounds/ClearDay.qml");

    bool isDayStyle = false; // make sure that if the background is definitively day, the colours match that

    if (currentWeather_->weatherIcon() == QStringLiteral("weather-clear")) {
        m_backgroundComponent = QStringLiteral("backgrounds/ClearDay.qml");
        isDayStyle = true;

    } else if (currentWeather_->weatherIcon() == QStringLiteral("weather-clear-night")) {
        m_backgroundComponent = QStringLiteral("backgrounds/ClearNight.qml");

    } else if (currentWeather_->weatherIcon() == QStringLiteral("weather-clouds")) {
        m_backgroundComponent = QStringLiteral("backgrounds/CloudyDay.qml");
        isDayStyle = true;

    } else if (currentWeather_->weatherIcon() == QStringLiteral("weather-clouds-night") || currentWeather_->weatherIcon() == QStringLiteral("weather-overcast")) {
        m_backgroundComponent = QStringLiteral("backgrounds/CloudyNight.qml");

    } else if (currentWeather_->weatherIcon() == QStringLiteral("weather-few-clouds")) {
        m_backgroundComponent = QStringLiteral("backgrounds/PartlyCloudyDay.qml");
        isDayStyle = true;

    } else if (currentWeather_->weatherIcon() == QStringLiteral("weather-few-clouds-night")) {
        m_backgroundComponent = QStringLiteral("backgrounds/PartlyCloudyNight.qml");

    } else if (currentWeather_->weatherIcon() == QStringLiteral("weather-fog") || currentWeather_->weatherIcon() == QStringLiteral("weather-mist")) {
        m_backgroundComponent = QStringLiteral("backgrounds/Misty.qml");
        isDayStyle = true;

    } else if (currentWeather_->weatherIcon() == QStringLiteral("weather-freezing-rain") || currentWeather_->weatherIcon() == QStringLiteral("weather-snow-hail") || currentWeather_->weatherIcon() == QStringLiteral("weather-showers") ||
               currentWeather_->weatherIcon() == QStringLiteral("weather-showers-day") || currentWeather_->weatherIcon() == QStringLiteral("weather-showers-scattered") ||
               currentWeather_->weatherIcon() == QStringLiteral("weather-showers-scattered-day") || currentWeather_->weatherIcon() == QStringLiteral("weather-storm") ||
               currentWeather_->weatherIcon() == QStringLiteral("weather-storm-day")) {
        m_backgroundComponent = QStringLiteral("backgrounds/RainyDay.qml");
        isDayStyle = true;

    } else if (currentWeather_->weatherIcon() == QStringLiteral("weather-showers-night") || currentWeather_->weatherIcon() == QStringLiteral("weather-showers-scattered-night") ||
               currentWeather_->weatherIcon() == QStringLiteral("weather-storm-night")) {
        m_backgroundComponent = QStringLiteral("backgrounds/RainyNight.qml");

    } else if (currentWeather_->weatherIcon() == QStringLiteral("weather-hail") || currentWeather_->weatherIcon() == QStringLiteral("weather-snow-scattered") || currentWeather_->weatherIcon() == QStringLiteral("weather-snow")) {
        m_backgroundComponent = QStringLiteral("backgrounds/SnowyDay.qml");
        isDayStyle = true;

    } else if (currentWeather_->weatherIcon() == QStringLiteral("weather-snow-scattered-night")) {
        m_backgroundComponent = QStringLiteral("backgrounds/SnowyNight.qml");
    }

    if (isDayStyle) {
        m_cardBackgroundColor = QStringLiteral("#fefefe");
        m_cardTextColor = QStringLiteral("black");
        m_backgroundColor = QStringLiteral("#3daee2");
        m_textColor = m_cardTextColor;
        m_iconColor = QStringLiteral("eff0f1");
    } else {
        m_cardBackgroundColor = QStringLiteral("#333333");
        m_cardTextColor = QStringLiteral("#eeeeee");
        m_backgroundColor = QStringLiteral("#222222");
        m_textColor = m_cardTextColor;
        m_iconColor = QStringLiteral("white");
    }
}

void WeatherLocation::initData(AbstractWeatherForecast fc)
{
    forecast_ = fc;
    weatherBackendProvider_->setCurrentData(forecast_);
    weatherBackendProvider_->setCurrentSunriseData(fc.sunrise());
    weatherBackendProvider_->fetchSunriseData(); // TODO detect if we need to actually fetch sunrise data
    determineCurrentForecast();
    emit weatherRefresh(forecast_);
    emit propertyChanged();
}

void WeatherLocation::update()
{
    weatherBackendProvider_->update();
}

void WeatherLocation::writeToCache(AbstractWeatherForecast &fc)
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
QJsonDocument WeatherLocation::convertToJson(AbstractWeatherForecast &fc)
{
    QJsonDocument doc;
    doc.setObject(fc.toJson());
    return doc;
}

void WeatherLocation::changeBackend(Kweather::Backend backend)
{
    if (backend != backend_) {
        auto old = weatherBackendProvider_;
        backend_ = backend;
        AbstractWeatherAPI *tmp = nullptr;
        switch (backend_) {
        case Kweather::Backend::OWM:
            tmp = new OWMWeatherAPI(this->locationId(), this->timeZone(), this->latitude_, this->longitude());
            break;
        case Kweather::Backend::NMI:
            tmp = new NMIWeatherAPI2(this->locationId(), this->timeZone(), this->latitude_, this->longitude());
            break;
        default:
            return;
        }
        weatherBackendProvider_ = tmp;
        connect(this->weatherBackendProvider(), &AbstractWeatherAPI::updated, this, &WeatherLocation::updateData, Qt::UniqueConnection);
        weatherBackendProvider_->setCurrentSunriseData(old->currentSunriseData());
        weatherBackendProvider_->fetchSunriseData();
        this->update();
        old->deleteLater();
    }
}

void WeatherLocation::updateChart()
{
    m_maxTempList.clear();
    m_xAxisList.clear();
    for (const auto &day : forecast_.dailyForecasts()) {
        m_maxTempList.append(day.maxTemp());
        m_xAxisList.append(QLocale::system().toString(day.date(),QStringLiteral("ddd")));
    }

    Q_EMIT chartListChanged();
}

WeatherLocation::~WeatherLocation()
{
    delete weatherBackendProvider_;
}

const QVariantList &WeatherLocation::maxTempList()
{
    return m_maxTempList;
}
const QVariantList& WeatherLocation::xAxisList()
{
    return m_xAxisList;
}

void WeatherLocation::updateCurrentDateTime()
{
    Q_EMIT currentTimeChanged();
    Q_EMIT currentDateChanged();
}
