#include "abstracthourlyweatherforecast.h"

#include <utility>
AbstractHourlyWeatherForecast::AbstractHourlyWeatherForecast(QDateTime date,
                                                             QString weatherDescription,
                                                             QString weatherIcon,
                                                             QString neutralWeatherIcon,
                                                             float temperature,
                                                             float pressure,
                                                             AbstractHourlyWeatherForecast::WindDirection windDirection,
                                                             float windSpeed,
                                                             float humidity,
                                                             float fog,
                                                             float uvIndex,
                                                             float precipitationAmount) :
    date_(std::move(date)),
    weatherDescription_(std::move(weatherDescription)),
    weatherIcon_(weatherIcon),
    neutralWeatherIcon_(neutralWeatherIcon),
    temperature_(temperature),
    pressure_(pressure),
    windDirection_(windDirection),
    windSpeed_(windSpeed),
    humidity_(humidity),
    fog_(fog),
    uvIndex_(uvIndex),
    precipitationAmount_(precipitationAmount),
{
}
