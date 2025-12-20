/*
    SPDX-FileCopyrightText: 2020 HanY <hanyoung@protonmail.com>
    SPDX-FileCopyrightText: 2025 Devin Lin <devin@kde.org>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/
#include "kweatherplasmoid.h"

#include <QQmlApplicationEngine>
#include <QTimer>

#include <KSharedConfig>

#include "hourlymodel.h"
#include "kweathersettings.h"

using namespace Qt::StringLiterals;

KWeatherPlasmoid::KWeatherPlasmoid(QObject *parent, const KPluginMetaData &md, const QVariantList &args)
    : Plasma::Applet(parent, md, args)
    , m_hourlyModel(new HourlyModel(this))
    , m_queryModel(new LocationQueryModel())
    , m_timer(new QTimer(this))
{
    auto group = config().group(u"General"_s);
    m_locationName = group.readEntry(u"LocationName"_s);

    if (!m_locationName.isEmpty()) {
        m_latitude = group.readEntry(u"Latitude"_s).toDouble();
        m_longitude = group.readEntry(u"Longitude"_s).toDouble();
        m_needLocation = false;
        update();
        m_timer->setInterval(3600 * 1000);
        m_timer->start();
        connect(m_timer, &QTimer::timeout, this, [this] {
            update();
        });
    }
}

void KWeatherPlasmoid::update()
{
    auto pendingForecast = m_source.requestData(m_latitude, m_longitude);
    connect(pendingForecast, &KWeatherCore::PendingWeatherForecast::finished, this, [this, pendingForecast] {
        m_forecast = pendingForecast->value();
        pendingForecast->deleteLater();
        m_hourlyModel->loadForecast(m_forecast);
        Q_EMIT updated();
    });
}

void KWeatherPlasmoid::setLocation(const QString &locationName, double latitude, double longitude)
{
    m_locationName = locationName;
    m_latitude = latitude;
    m_longitude = longitude;

    auto group = config().group(u"General"_s);
    group.writeEntry(u"LocationName"_s, m_locationName);
    group.writeEntry(u"Latitude"_s, m_latitude);
    group.writeEntry(u"Longitude"_s, m_longitude);

    update();
    m_needLocation = false;
    Q_EMIT needLocationChanged();
    Q_EMIT weatherLocationChanged();

    config().sync();
}

bool KWeatherPlasmoid::hasForecast() const
{
    return !m_forecast.dailyWeatherForecast().empty() && !m_forecast.dailyWeatherForecast().front().hourlyWeatherForecast().empty();
}

const KWeatherCore::HourlyWeatherForecast &KWeatherPlasmoid::getFirst() const
{
    return m_forecast.dailyWeatherForecast().front().hourlyWeatherForecast().front();
}

QString KWeatherPlasmoid::locationName() const
{
    return m_locationName;
}

qreal KWeatherPlasmoid::humidity() const
{
    if (!hasForecast()) {
        return 0;
    }
    return getFirst().humidity();
}

QString KWeatherPlasmoid::weatherIcon() const
{
    if (!hasForecast()) {
        return QStringLiteral("weather-none-available-symbolic");
    }
    return getFirst().weatherIcon();
}

QString KWeatherPlasmoid::desc() const
{
    if (!hasForecast()) {
        return {};
    }
    return getFirst().weatherDescription();
}

qreal KWeatherPlasmoid::temp() const
{
    if (!hasForecast()) {
        return 0;
    }
    return getFirst().temperature();
}

qreal KWeatherPlasmoid::precipitation() const
{
    if (!hasForecast()) {
        return 0;
    }
    return getFirst().precipitationAmount();
}

QColor KWeatherPlasmoid::topColor() const
{
    if (!hasForecast()) {
        return {};
    }

    auto icon = weatherIcon();

    // Use a hash for efficient string comparison in switch-like pattern
    if (icon == u"weather-clear"_s) {
        return {255, 193, 7}; //"#ffc107"
    } else if (icon == u"weather-clear-night"_s) {
        return {69, 90, 100}; // "#455a64"
    } else if (icon == u"weather-clouds"_s || icon == u"weather-few-clouds"_s) {
        return {3, 155, 229}; // #039be5
    } else if (icon == u"weather-clouds-night"_s || icon == u"weather-overcast"_s) {
        return {69, 90, 100}; // #455a64
    } else if (icon == u"weather-few-clouds-night"_s) {
        return {69, 90, 100}; // "#455a64"
    } else if (icon == u"weather-fog"_s || icon == u"weather-mist"_s) {
        return {194, 197, 203}; // #c2c5cb
    } else if (icon == u"weather-freezing-rain"_s || icon == u"weather-snow-hail"_s || icon == u"weather-showers"_s || icon == u"weather-showers-day"_s
               || icon == u"weather-showers-scattered"_s || icon == u"weather-showers-scattered-day"_s || icon == u"weather-storm"_s
               || icon == u"weather-storm-day"_s) {
        return {54, 156, 203}; // #369ccb
    } else if (icon == u"weather-showers-night"_s || icon == u"weather-showers-scattered-night"_s || icon == u"weather-storm-night"_s) {
        return {69, 90, 100}; // #455a64
    } else if (icon == u"weather-hail"_s || icon == u"weather-snow-scattered"_s || icon == u"weather-snow"_s) {
        return {53, 217, 237}; // #35d9ed
    } else if (icon == u"weather-snow-scattered-night"_s) {
        return {69, 90, 100}; // #455a64
    }

    return {};
}

QColor KWeatherPlasmoid::bottomColor() const
{
    if (!hasForecast()) {
        return {};
    }

    auto icon = weatherIcon();

    if (icon == u"weather-clear"_s) {
        return {255, 193, 7};
    } else if (icon == u"weather-clear-night"_s) {
        return {38, 50, 56}; // "#263238"
    } else if (icon == u"weather-clouds"_s || icon == u"weather-few-clouds"_s) {
        return {2, 119, 189}; // #0277bd
    } else if (icon == u"weather-clouds-night"_s || icon == u"weather-overcast"_s) {
        return {38, 50, 56}; // #263238
    } else if (icon == u"weather-few-clouds-night"_s) {
        return {38, 50, 56}; // "#263238"
    } else if (icon == u"weather-fog"_s || icon == u"weather-mist"_s) {
        return {91, 96, 107}; // #5b606b
    } else if (icon == u"weather-freezing-rain"_s || icon == u"weather-snow-hail"_s || icon == u"weather-showers"_s || icon == u"weather-showers-day"_s
               || icon == u"weather-showers-scattered"_s || icon == u"weather-showers-scattered-day"_s || icon == u"weather-storm"_s
               || icon == u"weather-storm-day"_s) {
        return {26, 131, 179}; // #1A83B3
    } else if (icon == u"weather-showers-night"_s || icon == u"weather-showers-scattered-night"_s || icon == u"weather-storm-night"_s) {
        return {38, 50, 56}; // #263238
    } else if (icon == u"weather-hail"_s || icon == u"weather-snow-scattered"_s || icon == u"weather-snow"_s) {
        return {36, 163, 222}; // #24a3de
    } else if (icon == u"weather-snow-scattered-night"_s) {
        return {38, 50, 56}; // #263238
    }

    return {};
}

bool KWeatherPlasmoid::needLocation() const
{
    return m_needLocation;
}

HourlyModel *KWeatherPlasmoid::hourlyModel() const
{
    return m_hourlyModel;
}

LocationQueryModel *KWeatherPlasmoid::queryModel() const
{
    return m_queryModel;
}

K_PLUGIN_CLASS_WITH_JSON(KWeatherPlasmoid, "metadata.json")

#include "kweatherplasmoid.moc"

#include "moc_kweatherplasmoid.cpp"
