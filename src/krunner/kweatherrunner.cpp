/*
 * SPDX-FileCopyrightText: 2026 The KWeather Authors
 * SPDX-FileCopyrightText: 2026 Arran Ubels <kde@arran4.com>
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "kweatherrunner.h"

#include <QEventLoop>
#include <QIcon>
#include <QProcess>
#include <QTimer>
#include <algorithm>
#include <cmath>

#include <KConfigGroup>
#include <KLocalizedString>
#include <KSharedConfig>

#include <KWeatherCore/LocationQuery>
#include <KWeatherCore/LocationQueryReply>
#include <KWeatherCore/PendingWeatherForecast>
#include <KWeatherCore/WeatherForecastSource>

K_PLUGIN_CLASS_WITH_JSON(KWeatherRunner, "kweatherrunner.json")

KWeatherRunner::KWeatherRunner(QObject *parent, const KPluginMetaData &metaData)
    : KRunner::AbstractRunner(parent, metaData)
{
}

KWeatherRunner::~KWeatherRunner() = default;

void KWeatherRunner::match(KRunner::RunnerContext &context)
{
    QString query = context.query().trimmed();
    QString cmd = i18nc("Command to get weather", "weather");
    QString engCmd = QStringLiteral("weather");

    QString locationStr;
    if (query.compare(cmd, Qt::CaseInsensitive) == 0 || query.compare(engCmd, Qt::CaseInsensitive) == 0) {
        locationStr = QString();
    } else if (query.startsWith(cmd + QLatin1Char(' '), Qt::CaseInsensitive)) {
        locationStr = query.mid(cmd.length() + 1).trimmed();
    } else if (query.startsWith(engCmd + QLatin1Char(' '), Qt::CaseInsensitive)) {
        locationStr = query.mid(engCmd.length() + 1).trimmed();
    } else {
        return; // Does not match exactly "weather" or "weather <location>"
    }

    if (locationStr.isEmpty()) {
        // Look up saved locations from KWeather settings
        KSharedConfigPtr config = KSharedConfig::openConfig(QStringLiteral("kweather/kweatherrc"));
        KConfigGroup locationsGroup = config->group(QStringLiteral("WeatherLocations"));
        QStringList groups = locationsGroup.groupList();

        if (groups.isEmpty()) {
            return;
        }

        // Fetch at most 3 locations to avoid stalling the runner thread
        int limit = std::min<qsizetype>(groups.size(), 3);
        for (int i = 0; i < limit; ++i) {
            KConfigGroup group = locationsGroup.group(groups.at(i));
            double lat = group.readEntry("latitude", 0.0);
            double lon = group.readEntry("longitude", 0.0);
            QString name = group.readEntry("locationName", QString());

            fetchAndAddWeather(context, lat, lon, name);
        }
    } else {
        // Query location using KWeatherCore
        KWeatherCore::LocationQuery locationQuery;
        KWeatherCore::LocationQueryReply *reply = locationQuery.query(locationStr);

        QEventLoop loop;
        QTimer::singleShot(2000, &loop, &QEventLoop::quit);
        QObject::connect(reply, &KWeatherCore::LocationQueryReply::finished, &loop, &QEventLoop::quit);
        loop.exec();

        if (reply->error() == KWeatherCore::LocationQueryReply::NoError) {
            auto results = reply->result();
            if (!results.empty()) {
                // Fetch for the top result
                const auto &res = results.front();
                fetchAndAddWeather(context, res.latitude(), res.longitude(), res.toponymName());
            }
        }
        delete reply;
    }
}

void KWeatherRunner::fetchAndAddWeather(KRunner::RunnerContext &context, double lat, double lon, const QString &locationName)
{
    KWeatherCore::WeatherForecastSource source;
    KWeatherCore::PendingWeatherForecast *pending = source.requestData(lat, lon);

    QEventLoop loop;
    QTimer::singleShot(2000, &loop, &QEventLoop::quit);
    QObject::connect(pending, &KWeatherCore::PendingWeatherForecast::finished, &loop, &QEventLoop::quit);
    loop.exec();

    KWeatherCore::WeatherForecast forecast = pending->value();
    delete pending;

    if (forecast.dailyWeatherForecast().empty()) {
        return;
    }

    auto daily = forecast.dailyWeatherForecast().front();
    if (daily.hourlyWeatherForecast().empty()) {
        return;
    }

    auto current = daily.hourlyWeatherForecast().front();

    int temp = std::round(current.temperature());
    QString description = current.weatherDescription();
    QString icon = current.weatherIcon();

    int maxTemp = std::round(daily.maxTemp());
    int minTemp = std::round(daily.minTemp());

    KRunner::QueryMatch match(this);
    match.setId(QStringLiteral("kweather_") + locationName);
    match.setText(i18nc("Weather forecast: %1 is temperature, %2 is description, %3 is location", "%1°C - %2 in %3", temp, description, locationName));
    match.setSubtext(i18nc("Weather forecast subtext: %1 is high temp, %2 is low temp", "High: %1°C Low: %2°C", maxTemp, minTemp));
    match.setIconName(icon);
    match.setRelevance(1.0);
    match.setCategoryRelevance(KRunner::QueryMatch::CategoryRelevance::Highest);

    // Store coordinates to use in run() if needed, though we just open kweather right now
    match.setData(QVariantList{lat, lon});

    context.addMatch(match);
}

void KWeatherRunner::run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match)
{
    Q_UNUSED(context);
    Q_UNUSED(match);

    // KWeather does not currently support CLI arguments for specific locations
    QProcess::startDetached(QStringLiteral("kweather"), {});
}

#include "kweatherrunner.moc"