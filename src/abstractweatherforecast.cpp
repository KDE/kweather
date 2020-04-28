#include "abstractweatherforecast.h"
#include <limits>
AbstractWeatherforecast::AbstractWeatherforecast(QString & LocationName,
                                                 QString & WindDirection,
                                                 QString & WeatherDescription,
                                                 QString & WeatherIcon,
                                                 QDateTime Time,
                                                 float     Lat,
                                                 float     Lon,
                                                 float     Precipitation = -1.0,
                                                 float     Fog           = -1.0,
                                                 int       Clouldiness   = -1,
                                                 int       WindSpeed     = -1,
                                                 int       MaxTemp       = std::numeric_limits<int>::min(),
                                                 int       MinTemp       = std::numeric_limits<int>::max(),
                                                 int       Humidity      = -1,
                                                 int       Pressure      = -1) :
  locationName_(LocationName),
  windDirection_(WindDirection),
  weatherDescription_(WeatherDescription),
  weatherIcon_(WeatherIcon),
  time_(Time),
  lat_(Lat),
  lon_(Lon),
  precipitation_(Precipitation),
  fog_(Fog),
  clouldiness_(Clouldiness),
  windSpeed_(WindSpeed),
  maxTemp_(MaxTemp),
  minTemp_(MinTemp),
  humidity_(Humidity),
  pressure_(Pressure) {}

const QString& AbstractWeatherforecast::locationName() {
  return locationName_;
}

const QString& AbstractWeatherforecast::windDirection() {
  return windDirection_;
}

const QString& AbstractWeatherforecast::weatherDescription() {
  return weatherDescription_;
}

const QString& AbstractWeatherforecast::weatherIcon() {
  return weatherIcon_;
}
