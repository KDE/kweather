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
class WeatherLocationListModel;
class WeatherforecastManager : public QObject {
    Q_OBJECT

public:

    static WeatherforecastManager& instance(WeatherLocationListModel& model);

signals:

    void updated();
private slots:
    void update();
protected:

    explicit WeatherforecastManager(WeatherLocationListModel& model,int defaultAPI = NORWEGIAN);

private:
    WeatherLocationListModel& model_;
    int api_ = NORWEGIAN;
    std::unique_ptr<std::vector<NorwegianMeteorologicalInstitute*>> norwegian_ = nullptr;
    std::unique_ptr<std::vector<OpenWeatherMap*>>openWeather_ = nullptr;

    void writeToCache(const std::vector<AbstractWeatherForecast *>& data);

    void readFromCache(QString& url);
    WeatherforecastManager(const WeatherforecastManager&);
    WeatherforecastManager& operator=(const WeatherforecastManager&);
};

#endif // WEATHERFORECASTMANAGER_H
