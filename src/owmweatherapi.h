/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef OPENWEATHERMAP_H
#define OPENWEATHERMAP_H
#include "abstractweatherapi.h"
#include "global.h"

#include <QObject>

using namespace Kweather;

// OpenWeatherMap API Implementation
// api.openweathermap.org
class OWMWeatherAPI : public AbstractWeatherAPI
{
    Q_OBJECT

public:
    OWMWeatherAPI(QString locationId, QString timeZone, double latitude, double longitude);
    ~OWMWeatherAPI();

    void update() override;
    void applySunriseDataToForecast() override;
signals:
    void TokenInvalid();
    void TooManyCalls();

private slots:

    void parse(QNetworkReply *Reply) override;

private:
    // map for weather ID to icon
    const QMap<QString, ResolvedWeatherDesc> apiDescMap = {
        {"01d", ResolvedWeatherDesc("weather-clear", i18n("Clear"))},
        {"01n", ResolvedWeatherDesc("weather-clear-night", i18n("Clear"))},
        {"02d", ResolvedWeatherDesc("weather-few-clouds", i18n("Mostly Sunny"))},
        {"02n", ResolvedWeatherDesc("weather-few-clouds-night", i18n("Mostly Sunny"))},
        {"03d", ResolvedWeatherDesc("weather-clouds", i18n("Partly Cloudy"))},
        {"03n", ResolvedWeatherDesc("weather-clouds-night", i18n("Partly Cloudy"))},
        {"04d", ResolvedWeatherDesc("weather-clouds", i18n("Partly Cloudy"))},
        {"04n", ResolvedWeatherDesc("weather-clouds-night", i18n("Partly Cloudy"))},
        {"09d", ResolvedWeatherDesc("weather-showers-day", i18n("Rain Showers"))},
        {"09n", ResolvedWeatherDesc("weather-showers-night", i18n("Rain Showers"))},
        {"10d", ResolvedWeatherDesc("weather-showers-day", i18n("Rain"))},
        {"10n", ResolvedWeatherDesc("weather-showers-night", i18n("Rain"))},
        {"11d", ResolvedWeatherDesc("weather-storm-day", i18n("Thunderstorm"))},
        {"11n", ResolvedWeatherDesc("weather-storm-night", i18n("Thunderstorm"))},
        {"13d", ResolvedWeatherDesc("weather-snow-scattered-day", i18n("Snow"))},
        {"13n", ResolvedWeatherDesc("weather-snow-scattered-night", i18n("Snow"))},
        {"50d", ResolvedWeatherDesc("weather-mist", i18n("Mist"))},
        {"50n", ResolvedWeatherDesc("weather-mist", i18n("Mist"))},
    };

    const QMap<QString, ResolvedWeatherDesc> neutralApiDescMap = {
        {"01d", ResolvedWeatherDesc("weather-clear", i18n("Clear"))},
        {"01n", ResolvedWeatherDesc("weather-clear", i18n("Clear"))},
        {"02d", ResolvedWeatherDesc("weather-few-clouds", i18n("Mostly Sunny"))},
        {"02n", ResolvedWeatherDesc("weather-few-clouds", i18n("Mostly Sunny"))},
        {"03d", ResolvedWeatherDesc("weather-clouds", i18n("Partly Cloudy"))},
        {"03n", ResolvedWeatherDesc("weather-clouds", i18n("Partly Cloudy"))},
        {"04d", ResolvedWeatherDesc("weather-clouds", i18n("Partly Cloudy"))},
        {"04n", ResolvedWeatherDesc("weather-clouds", i18n("Partly Cloudy"))},
        {"09d", ResolvedWeatherDesc("weather-showers", i18n("Rain Showers"))},
        {"09n", ResolvedWeatherDesc("weather-showers", i18n("Rain Showers"))},
        {"10d", ResolvedWeatherDesc("weather-showers", i18n("Rain"))},
        {"10n", ResolvedWeatherDesc("weather-showers", i18n("Rain"))},
        {"11d", ResolvedWeatherDesc("weather-storm", i18n("Thunderstorm"))},
        {"11n", ResolvedWeatherDesc("weather-storm", i18n("Thunderstorm"))},
        {"13d", ResolvedWeatherDesc("weather-snow", i18n("Snow"))},
        {"13n", ResolvedWeatherDesc("weather-snow", i18n("Snow"))},
        {"50d", ResolvedWeatherDesc("weather-mist", i18n("Mist"))},
        {"50n", ResolvedWeatherDesc("weather-mist", i18n("Mist"))},
    };
};
#endif // OPENWEATHERMAP_H
