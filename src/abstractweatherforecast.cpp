#include "abstractweatherforecast.h"
#include <limits>
#include <utility>

AbstractWeatherForecast::AbstractWeatherForecast() {}
AbstractWeatherForecast::AbstractWeatherForecast(QString locationName,
                                                 float latitude,
                                                 float longitude,
                                                 QList<AbstractHourlyWeatherForecast *> hourlyForecasts,
                                                 QList<AbstractDailyWeatherForecast *> dailyForecasts) :
    locationName_(std::move(locationName)),
    latitude_(latitude),
    longitude_(longitude),
    hourlyForecasts_(std::move(hourlyForecasts)),
    dailyForecasts_(std::move(dailyForecasts))
{
}

AbstractWeatherForecast::~AbstractWeatherForecast()
{
    for (auto hour : hourlyForecasts_) delete hour;
    for (auto day : dailyForecasts_) delete day;
}
