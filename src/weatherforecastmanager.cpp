#include "weatherforecastmanager.h"
#include "weatherlocationmodel.h"
#include <KConfigCore/KConfigGroup>
#include <QDirIterator>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QTimeZone>
#include <QTimer>

WeatherForecastManager::WeatherForecastManager(WeatherLocationListModel &model, int defaultAPI)
    : model_(model)
    , api_(defaultAPI)
{
    if (defaultAPI != NORWEGIAN && defaultAPI != OPENWEATHER) {
        qDebug() << "wrong api";
        exit(1);
    }

    // create cache location if it does not exist, and load cache
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    if (!dir.exists())
        dir.mkpath(".");
    readFromCache();

    distribution = new std::uniform_int_distribution<int>(0, 30 * 60); // uniform random update interval, 60 min to 90 min
    auto rand = std::bind(*distribution, generator);
    cacheTimer = new QTimer(this);
    cacheTimer->setSingleShot(true);
    connect(cacheTimer, &QTimer::timeout, this, &WeatherForecastManager::cache);

    updateTimer = new QTimer(this);
    updateTimer->setSingleShot(true);
    connect(updateTimer, &QTimer::timeout, this, &WeatherForecastManager::update);
    if (api_ == NORWEGIAN)
        updateTimer->start(0); // update when open
    else
        updateTimer->start(1000 * 3 * 3600 + rand() * 1000);
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
    updateTimer->start(1000 * 3600 + rand() * 1000); // reset timer

    cacheTimer->start(1000 * 60); // cache after 60 sec.
}

void WeatherForecastManager::writeToCache(AbstractWeatherForecast* data)
{
    QFile file;
    QString url;
    url = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir dir(url.append(QString("/cache/%1/%2/").arg(QString::number(static_cast<int>(data->latitude() * 100))).arg(QString::number(static_cast<int>(data->longitude() * 100))))); // create cache location
    if (!dir.exists())
        dir.mkpath(".");
    // should be this path: /home/user/.cache/kweather/7000/3000 for location with coordinate 70.00 30.00
    file.setFileName(dir.path() + "/cache.json");
    qDebug() << file.fileName();
    file.open(QIODevice::WriteOnly);
    file.write(convertToJson(data).toJson(QJsonDocument::Compact)); // write json
    file.close();
}

void WeatherForecastManager::readFromCache()
{
    QHash<WeatherLocation*, AbstractWeatherForecast*> map;
    for (auto wl : model_.getList()) map[wl] = nullptr;

    QFile reader;
    QDirIterator It(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/cache", QDirIterator::Subdirectories); // list directory entries
    while (It.hasNext()) {
        reader.setFileName(It.next());
        if (reader.fileName().right(10) != QLatin1String("cache.json")) // if not cache file ignore
            continue;
        reader.open(QIODevice::ReadOnly | QIODevice::Text);

        // obtain weather forecast
        AbstractWeatherForecast* fc = convertFromJson(reader.readAll());

        // loop over existing locations and add cached weather forecast data if location found
        for (auto wl : model_.getList()) {
            if (fc->locationId() == wl->locationId()) {
                // add forecast if it does not exist, or is newer than existing data
                if (map[wl] == nullptr || map[wl]->timeCreated() < fc->timeCreated()) {
                    map[wl] = fc;
                }
                break;
            }
        }

        // TODO delete if forecast was not used
        reader.close();
    }

    // add loaded locations from cache
    for (auto wl : model_.getList()) {
        if (map[wl] != nullptr) {
            wl->updateData(map[wl]);
        }
    }
}

QJsonDocument WeatherForecastManager::convertToJson(AbstractWeatherForecast* fc) // Qt uses QByteArray internally, pass by copy
{
    QJsonDocument doc;
    doc.setObject(fc->toJson());
    return doc;
}

AbstractWeatherForecast* WeatherForecastManager::convertFromJson(QByteArray data)
{
    QJsonObject doc = QJsonDocument::fromJson(data).object();
    return AbstractWeatherForecast::fromJson(doc);
}

void WeatherForecastManager::cache()
{
    qDebug() << "start caching";
    auto list = model_.getList();
    if (list.isEmpty())
        return;
    for (auto lc : model_.getList()) {
        if (lc->forecast() != nullptr)
            writeToCache(lc->forecast());
    }
}
