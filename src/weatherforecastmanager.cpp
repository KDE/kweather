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
    for (auto fc : data.forecasts()) {
        file.setFileName(QString(url + QString::number(fc->time().toSecsSinceEpoch()))); // file name are unix timestamp
        file.open(QIODevice::WriteOnly);                                                 // wipe out old data as well
        file.write((char *)fc, sizeof(*fc));                                             // write binary data into file
        file.close();                                                                    // we could do some optimizations here
    }                                                                                    // I just realised that one file for each
} // instance will occupy a lot of space even for a relative small block size, I will come up with a better idea later

void WeatherForecastManager::readFromCache()
{
    QFile reader;
    QDirIterator LatIt(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QLatin1String("/kweather")); // list directory entries
    while (LatIt.hasNext()) {                                                                                         // list all longitude
        QDirIterator LonIt(LatIt.next());
        while (LonIt.hasNext()) {
            QDirIterator forecastIt(LonIt.next(), QDirIterator::Subdirectories);
            // stop here
        }
    }
}
