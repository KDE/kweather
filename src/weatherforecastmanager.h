#ifndef WEATHERFORECASTMANAGER_H
#define WEATHERFORECASTMANAGER_H

// ####### API MACRO #######
#define NORWEGIAN 0
#define OPENWEATHER 1

// #########################

#include <QObject>
#include <memory>
#include <random>
#include <vector>
class AbstractWeatherForecast;
class NMIWeatherAPI2;
class WeatherLocationListModel;
class QTimer;
class WeatherLocation;
class WeatherForecastManager : public QObject
{
    Q_OBJECT

public:
    static WeatherForecastManager &instance(WeatherLocationListModel &model);

signals:

    void updated();
public slots:
    void cache();
private slots:
    void update();

private:
    std::default_random_engine generator;
    std::uniform_int_distribution<int> *distribution;
    WeatherLocationListModel &model_;
    int api_ = NORWEGIAN;
    QJsonDocument convertToJson(AbstractWeatherForecast* lc);
    AbstractWeatherForecast* convertFromJson(QByteArray data);
    void writeToCache(AbstractWeatherForecast* data);
    QTimer *updateTimer;
    QTimer *cacheTimer;
    void readFromCache();
    WeatherForecastManager(WeatherLocationListModel &model, int defaultAPI = NORWEGIAN);
    WeatherForecastManager(const WeatherForecastManager &);
    WeatherForecastManager &operator=(const WeatherForecastManager &);
};

#endif // WEATHERFORECASTMANAGER_H
