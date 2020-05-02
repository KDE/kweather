#include "abstractweatherforecast.h"
#include <limits>
AbstractWeatherForecast::AbstractWeatherForecast(QString LocationName, QString WindDirection,
                                                 QString WeatherDescription, QString WeatherIcon, QDateTime Time,
                                                 float Lat, float Lon, float Precipitation = -1.0, float Fog = -1.0,
                                                 int Cloudiness = -1, int WindSpeed = -1,
                                                 int MaxTemp = std::numeric_limits<int>::min(),
                                                 int MinTemp = std::numeric_limits<int>::max(), int Humidity = -1,
                                                 int Pressure = -1)
    : locationName_(LocationName)
    , windDirection_(WindDirection)
    , weatherDescription_(WeatherDescription)
    , weatherIcon_(WeatherIcon)
    , time_(Time)
    , latitude_(Lat)
    , longitude_(Lon)
    , precipitation_(Precipitation)
    , fog_(Fog)
    , cloudiness_(Cloudiness)
    , windSpeed_(WindSpeed)
    , maxTemp_(MaxTemp)
    , minTemp_(MinTemp)
    , humidity_(Humidity)
    , pressure_(Pressure)
{
}

AbstractWeatherForecast::AbstractWeatherForecast() {}

const QString& AbstractWeatherForecast::locationName()
{
    return locationName_;
}

const QString& AbstractWeatherForecast::windDirection()
{
    return windDirection_;
}

const QString& AbstractWeatherForecast::weatherDescription()
{
    return weatherDescription_;
}

const QString& AbstractWeatherForecast::weatherIcon()
{
    return weatherIcon_;
}
