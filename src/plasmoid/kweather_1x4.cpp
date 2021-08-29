/*
    SPDX-FileCopyrightText: 2020 HanY <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/
#include "kweather_1x4.h"
#include "hourlymodel.h"
#include "kweathersettings.h"
#include <KSharedConfig>
#include <KWeatherCore/WeatherForecastSource>
#include <QQmlApplicationEngine>
KWeather_1x4::KWeather_1x4(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
    , m_hourlyModel(new HourlyModel())
{
    qmlRegisterAnonymousType<HourlyModel>("HourlyModel", 1);
    auto config = KSharedConfig::openConfig(QStringLiteral("kweather/plasmoid"));
    auto group = config->group("general");
    QString locationID = group.readEntry("locationID");
    if (!locationID.isEmpty()) {
        auto m_config = KWeatherSettings::self()->config()->group("WeatherLocations");
        auto m_group = m_config.group(locationID);
        m_location = m_group.readEntry("locationName");
        m_latitude = m_group.readEntry("latitude").toDouble();
        m_longitude = m_group.readEntry("longitude").toDouble();
        m_needLocation = false;
        update();
    }
}

void KWeather_1x4::update()
{
    auto pendingForecast = KWeatherCore::WeatherForecastSource().requestData(m_latitude, m_longitude);
    if (pendingForecast->isFinished()) {
        m_forecast = pendingForecast->value();
        pendingForecast->deleteLater();
        m_hourlyModel->loadForecast(m_forecast);
        Q_EMIT updated();
    } else {
        connect(pendingForecast, &KWeatherCore::PendingWeatherForecast::finished, [this, pendingForecast] {
            m_forecast = pendingForecast->value();
            pendingForecast->deleteLater();
            m_hourlyModel->loadForecast(m_forecast);
            Q_EMIT updated();
        });
    }
}

QStringList KWeather_1x4::locationsInSystem()
{
    auto m_config = KWeatherSettings::self()->config()->group("WeatherLocations");
    QStringList list;

    for (const auto &loc : m_config.groupList()) {
        list += m_config.group(loc).readEntry("locationName");
    }
    return list;
}
void KWeather_1x4::setLocation(const QString &location)
{
    auto m_config = KWeatherSettings::self()->config()->group("WeatherLocations");
    for (const auto &loc : m_config.groupList()) {
        auto m_group = m_config.group(loc);
        if (location == m_group.readEntry("locationName")) {
            m_location = location;
            auto config = KSharedConfig::openConfig(QStringLiteral("kweather/plasmoid"));
            auto group = config->group("general");
            group.writeEntry("locationID", loc);
            m_latitude = m_group.readEntry("latitude").toDouble();
            m_longitude = m_group.readEntry("longitude").toDouble();
            update();
            m_needLocation = false;
            Q_EMIT needLocationChanged();
            Q_EMIT locationChanged();

            group.sync();
            break;
        }
    }
}
bool KWeather_1x4::hasForecast() const
{
    return !m_forecast.dailyWeatherForecast().empty() && !m_forecast.dailyWeatherForecast().front().hourlyWeatherForecast().empty();
}
const KWeatherCore::HourlyWeatherForecast &KWeather_1x4::getFirst() const
{
    return m_forecast.dailyWeatherForecast().front().hourlyWeatherForecast().front();
}
QString KWeather_1x4::location() const
{
    return m_location;
}
qreal KWeather_1x4::humidity() const
{
    if (hasForecast())
        return getFirst().humidity();
    else
        return 0;
}
QString KWeather_1x4::weatherIcon() const
{
    if (hasForecast())
        return getFirst().weatherIcon();
    else
        return QStringLiteral("unknown");
}
QString KWeather_1x4::desc() const
{
    if (hasForecast())
        return getFirst().weatherDescription();
    else
        return {};
}
qreal KWeather_1x4::temp() const
{
    if (hasForecast())
        return getFirst().temperature();
    else
        return 0;
}
qreal KWeather_1x4::precipitation() const
{
    if (hasForecast())
        return getFirst().precipitationAmount();
    else
        return 0;
}

K_EXPORT_PLASMA_APPLET_WITH_JSON(kweather_1x4, KWeather_1x4, "metadata.json")

#include "kweather_1x4.moc"
