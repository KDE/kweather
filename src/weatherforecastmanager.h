#ifndef WEATHERFORECASTMANAGER_H
#define WEATHERFORECASTMANAGER_H

// ####### API MACRO #######
#define NORWEGIAN 0
#define OPENWEATHER 1

// #########################

#include <QObject>
#include <memory>
#include <vector>
class AbstractWeatherForecast;
class NorwegianMeteorologicalInstitute;
class OpenWeatherMap;
class WeatherforecastManager : public QObject {
  Q_OBJECT

public:

  static WeatherforecastManager         & instance();
  std::vector<AbstractWeatherForecast *>& getData();
  Q_INVOKABLE void                        addLocation(float lat,
                                                      float lon);

public slots:

  void update();

protected:

  explicit WeatherforecastManager(int defaultAPI = NORWEGIAN);

private:

  int api_ =
    NORWEGIAN;
  std::unique_ptr<NorwegianMeteorologicalInstitute>norwegian_ =
    nullptr;
  std::unique_ptr<OpenWeatherMap>openWeather_ =
    nullptr;

  void                    writeToCache(
    const std::vector<AbstractWeatherForecast *>& data);

  void                    readFromCache(QString& url);
  WeatherforecastManager(const WeatherforecastManager&);
  WeatherforecastManager& operator=(const WeatherforecastManager&);
};

#endif // WEATHERFORECASTMANAGER_H
