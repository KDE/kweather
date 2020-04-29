#include "weatherforecastmanager.h"
#include "openweathermap.h"
#include "norwegianmeteorologicalinstitute.h"
#include "openweathermap.h"
#include <QDebug>
WeatherforecastManager::WeatherforecastManager(int defaultAPI) : api_(defaultAPI)
{
  switch (api_) {
  case NORWEGIAN:
    norwegian_.reset(new NorwegianMeteorologicalInstitute());
    break;

  case OPENWEATHER:
    openWeather_.reset(new OpenWeatherMap());
    break;

  default:
    qDebug() << "wrong api";
    exit(1);
  }
}

void                    WeatherforecastManager::addLocation(float lat, float lon) {} //
                                                                                     // geo
                                                                                     // class

WeatherforecastManager& WeatherforecastManager::instance() {
  static WeatherforecastManager singleton;

  return singleton;
}
