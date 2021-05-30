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
class LocationModel::Location : public QObject
{
    Q_OBJECT
public:
    Location(QObject *parent = nullptr)
        : QObject(parent)
    {}
    QString name;
    double latitude, longitude;
    int index = 0;
    QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> forecast;
    void update(QExplicitlySharedDataPointer<KWeatherCore::WeatherForecast> forecast) {
        this->forecast = forecast;
    }
public Q_SIGNAL:
    void updated();

public Q_SLOT:
    void updateFromConnection() {
        if (reply) {
            update(reply->value());
            reply->deleteLater();
            Q_EMIT updated();
        }
    }
    KWeatherCore::PendingWeatherForecast *reply = nullptr;
};
LocationModel::LocationModel()
    : m_timer(new QTimer(this))
{
    // load locations from kconfig
    auto config = KWeatherSettings().config()->group(WEATHER_LOCATIONS_CFG_GROUP);
    auto i {0};
    for (const auto &location : config.groupList()) {
        auto locationSubGroup = config.group(location);
        if (locationSubGroup.isValid()) {
            auto locationFromConfig = new Location(this);
            locationFromConfig->name = locationSubGroup.readEntry("locationName");
            locationFromConfig->latitude = locationSubGroup.readEntry("longitude").toDouble();
            m_locations.push_back(locationFromConfig);
            locationFromConfig->index = i;
            i++;
        }
    }

    connect(m_timer, &QTimer::timeout, this, &LocationModel::update);
    m_timer->setInterval(1000 * 60 * 60); // one hour
    m_timer->start();
    update();
}

void LocationModel::update()
{
    KWeatherCore::WeatherForecastSource source;
    auto index {0};
    for (auto location : m_locations) {
        auto reply = source.requestData(location->latitude, location->longitude);
        if (reply->isFinished()) {
            location->update(reply->value());
            reply->deleteLater();
        } else {
            location->reply = reply;
            connect(reply, &KWeatherCore::PendingWeatherForecast::finished, location, &LocationModel::Location::updateFromConnection);
        }
        index++;
    }
}

QHash<int, QByteArray> LocationModel::roleNames() const
{
    static QHash<int, QByteArray> hash = {{LocationName, "locationName"}, {Temperature, "temperature"}, {Icon, "icon"}, {Description, "description"}, {Precipitation, "precipitation"}};
    return hash;
}
QVariant LocationModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() > (int)m_locations.size())
        return {};

    auto row = index.row();

    KWeatherCore::HourlyWeatherForecast const *currentWeather = nullptr;
    if (m_locations.at(row)->forecast && m_locations.at(row)->forecast->dailyWeatherForecast().empty()) {
        const auto &daily = m_locations.at(row)->forecast->dailyWeatherForecast();
        if (!daily.front().hourlyWeatherForecast().empty()) {
            currentWeather = &daily.front().hourlyWeatherForecast().front();
        }
    }

    if (!currentWeather) {
        return {};
    }

    switch (role) {
        case LocationName:
            return m_locations.at(row)->name;
        case Temperature:
            return currentWeather->temperature();
        case Icon:
            return currentWeather->weatherIcon();
        case Description:
            return currentWeather->weatherDescription();
        case Precipitation:
            return currentWeather->precipitationAmount();
        default:
            return QVariant();
    }
}

int LocationModel::rowCount(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return m_locations.size();
}
Q_DECLARE_INTERFACE(LocationModel, "org.kde.LocationModel")
