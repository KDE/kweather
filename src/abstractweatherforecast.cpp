#include "abstractweatherforecast.h"
#include <limits>
#include <utility>

AbstractWeatherForecast::AbstractWeatherForecast() {}
AbstractWeatherForecast::AbstractWeatherForecast(QDateTime timeCreated,
                                                 QString locationName,
                                                 float latitude,
                                                 float longitude, QList<AbstractHourlyWeatherForecast *> hourlyForecasts, QList<AbstractDailyWeatherForecast *> dailyForecasts) :
    timeCreated_(timeCreated),
    locationName_(std::move(locationName)),
    latitude_(latitude),
    longitude_(longitude),
    hourlyForecasts_(hourlyForecasts),
    dailyForecasts_(dailyForecasts)
{
}

AbstractWeatherForecast::~AbstractWeatherForecast()
{
    for (auto hour : hourlyForecasts_) delete hour;
    for (auto day : dailyForecasts_) delete day;
}
