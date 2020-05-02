#include "weatherforecastmanager.h"
#include "nmiweatherapi.h"
#include "owmweatherapi.h"
#include "weatherlocationmodel.h"
#include <QDebug>
#include <QDirIterator>
#include <QFile>
#include <QStandardPaths>
#include <QTimer>

WeatherForecastManager::WeatherForecastManager(WeatherLocationListModel &model, int defaultAPI)
    : model_(model)
    , api_(defaultAPI)
{
    if (defaultAPI != NORWEGIAN && defaultAPI != OPENWEATHER) {
        qDebug() << "wrong api";
        exit(1);
    }
    distribution = new std::uniform_int_distribution<int>(0, 30 * 3600); // uniform random update interval, 60 min to 90 min
    auto rand = std::bind(*distribution, generator);

    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &WeatherForecastManager::update);
    if (api_ == NORWEGIAN)
        timer->start(1000 * 3600 + rand());
    else
        timer->start(1000 * 3 * 3600 + rand());
}

WeatherForecastManager &WeatherForecastManager::instance(WeatherLocationListModel &model)
{
    static WeatherForecastManager singleton(model);
    return singleton;
}
void WeatherForecastManager::update()
{
    auto locations = model_.getList();
    for (auto wLocation : locations) {
        wLocation->weatherBackendProvider()->setLocation(wLocation->latitude(), wLocation->longitude());
        wLocation->weatherBackendProvider()->update();
    }
}

void WeatherForecastManager::writeToCache(WeatherLocation &data)
{
    QFile file;
    QString url;
    url = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    url.append(QLatin1String("/kweather"));
    url.append(QString("/%1/%2/").arg(QString::number(static_cast<int>(data.latitude() * 100))).arg(QString::number(static_cast<int>(data.longitude() * 100))));
    // should be this path: /home/user/.cache/kweather/7000/3000 for location with coordinate 70.00 30.00
    file.setFileName(QString(url + QString::number(data.forecasts().back()->time().toSecsSinceEpoch()))); // file name is last forecast's unix time
    for (auto fc : data.forecasts()) {                                                                    // this will later be used to determine if we want this cache or not
        file.open(QIODevice::WriteOnly);                                                                  // wipe out old data as well
        file.write((char *)fc, sizeof(*fc));                                                              // write binary data into file
    }
    file.close();
}

// ##### Work In Progress #####

void WeatherForecastManager::readFromCache()
{
    int i = 0; // index for weatherlistmodel
    QFile reader;
    QDirIterator LatIt(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QLatin1String("/kweather")); // list directory entries
    while (LatIt.hasNext()) {                                                                                         // list all longitude
        QDirIterator LonIt(LatIt.next());
        while (LonIt.hasNext()) {
            // This is for each individual location
            auto location = new WeatherLocation(); // one cache file corresponds to one location
            reader.setFileName(LonIt.next());
            if (LonIt.next().toLong() <= QDateTime::currentSecsSinceEpoch()) {
                reader.remove();
                model_.insert(i, location); // insert empty location and we are done
                break;                      // if no usable cache, terminate
            }
            reader.open(QIODevice::ReadOnly);
            // allocate new forecasts
            QList<AbstractWeatherForecast *> forecasts;
            auto fc = new AbstractWeatherForecast();
            while (!reader.atEnd()) {
                reader.read((char *)fc, sizeof(fc));
                forecasts.push_back(fc);
            }
            location->updateData(forecasts);
            model_.insert(i, location); // insert location to weatherlistmodel
        }
        i++; // add one location count
    }
}
