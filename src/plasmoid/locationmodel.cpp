/*
    SPDX-FileCopyrightText: 2021 HanYoung <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/
#include <KSharedConfig>
#include <KConfigGroup>
#include <KWeatherCore/WeatherForecastSource>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTimer>

#include "kweathersettings.h"
#include "locationmodel.h"
const QString WEATHER_LOCATIONS_CFG_GROUP = QStringLiteral("WeatherLocations");
const QString WEATHER_LOCATIONS_CFG_KEY = QStringLiteral("m_locations");
LocationModel::LocationModel()
    : m_timer(new QTimer(this))
{
    // load locations from kconfig
    auto config = KSharedConfig::openConfig(QString(), KSharedConfig::FullConfig, QStandardPaths::AppConfigLocation);
    KConfigGroup group = config->group(WEATHER_LOCATIONS_CFG_GROUP);
    QJsonDocument doc = QJsonDocument::fromJson(group.readEntry(WEATHER_LOCATIONS_CFG_KEY, "{}").toUtf8());
    const auto &array = doc.array();
    m_locations.resize(array.size());
    std::transform(array.begin(), array.end(), m_locations.begin(), [](const QJsonValue &val) -> Location{
        return {val[QStringLiteral("locationName")].toString(), val[QStringLiteral("latitude")].toDouble(), val[QStringLiteral("longitude")].toDouble()};
    });

    connect(m_timer, &QTimer::timeout, this, &LocationModel::update);
    m_timer->setInterval(1000 * 60 * 60); // one hour
    m_timer->start();
    update();
}

void LocationModel::update()
{
    if (m_forecasts.size() != m_locations.size())
        m_forecasts.resize(m_locations.size());

    KWeatherCore::WeatherForecastSource source;
    auto index {0};
    for (const auto &[name, lat, lgn] : m_locations) {
        auto reply = source.requestData(lat, lgn);
        if (reply->isFinished()) {
            m_forecasts.at(index) = reply->value();
            reply->deleteLater();
        } else {
            connect(reply, &KWeatherCore::PendingWeatherForecast::finished, this, [reply, this]{
                updateData(reply->value());
                reply->deleteLater();
            });
        }
        index++;
    }
}
void LocationModel::updateData(QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> forecast)
{
    auto lat = forecast->latitude();
    auto lgn = forecast->longitude();
    auto i {0};
    for (auto &loc : m_locations) {
        if (abs(loc.latitude - lat) <= std::numeric_limits<double>::epsilon() && abs(loc.longitude - lgn) <= std::numeric_limits<double>::epsilon()) {
            m_forecasts.at(i) = forecast;
            Q_EMIT dataChanged(index(i), index(i));
            break;
        } else
            i++;
    }
}
QHash<int, QByteArray> LocationModel::roleNames() const
{
    static QHash<int, QByteArray> hash = {{LocationName, "locationName"}, {Temperature, "temperature"}, {Icon, "icon"}, {Description, "description"}, {Precipitation, "precipitation"}};
    return hash;
}
QVariant LocationModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() > (int)m_forecasts.size())
        return {};

    auto row = index.row();

    KWeatherCore::HourlyWeatherForecast const *currentWeather = nullptr;
    if (!m_forecasts.at(row)->dailyWeatherForecast().empty()) {
        const auto &daily = m_forecasts.at(row)->dailyWeatherForecast();
        if (!daily.front().hourlyWeatherForecast().empty()) {
            currentWeather = &daily.front().hourlyWeatherForecast().front();
        }
    }

    switch (role) {
        case LocationName:
            return m_locations.at(row).name;
        case Temperature:
            return currentWeather ? currentWeather->temperature() : QVariant();
        case Icon:
            return  currentWeather ? currentWeather->weatherIcon() : QVariant();
        case Description:
            return currentWeather ? currentWeather->weatherDescription() : QVariant();
        case Precipitation:
            return currentWeather ? currentWeather->precipitationAmount() : QVariant();
        default:
            return QVariant();
    }
}

int LocationModel::rowCount(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return m_forecasts.size();
}
