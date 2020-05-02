#include "weatherforecastmanager.h"
#include "nmiweatherapi.h"
#include "owmweatherapi.h"
#include "weatherlocationmodel.h"
#include <QDebug>
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
    file.open(QIODevice::WriteOnly);                                                                      // this will later be used to determine if we want this cache or not
    file.write(convertToJson(data).toBinaryData());                                                       // write json
    file.close();
}

// ##### Work In Progress #####

void WeatherForecastManager::readFromCache()
{
    int i = 0; // index for weatherlistmodel
    QFile reader;
    QDirIterator LatIt(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QLatin1String("/kweather"), QDir::NoDotAndDotDot); // list directory entries
    while (LatIt.hasNext()) {                                                                                                               // list all longitude
        QDirIterator LonIt(LatIt.next(), QDir::Files);
        while (LonIt.hasNext()) {
            // This is for each individual location
            reader.setFileName(LonIt.next());
            if (reader.fileName().toLong() <= QDateTime::currentSecsSinceEpoch()) { // TODO: potential bug, has multiple cache but we only catch outdated one
                reader.remove();
                break; // if no usable cache, terminate
            }
            reader.open(QIODevice::ReadOnly);
            model_.getList() = convertFromJson(reader.readAll()); // Not sure, potential bug
        }
        i++; // add one location count
    }
}

QJsonDocument WeatherForecastManager::convertToJson(WeatherLocation &lc) // Qt uses QByteArray internally, pass by copy
{
    QJsonObject info;
    info.insert(QLatin1String("name"), lc.locationName());
    info.insert(QLatin1String("latitude"), lc.latitude());
    info.insert(QLatin1String("longitude"), lc.longitude());
    info.insert(QLatin1String("timezone"), lc.weatherBackendProvider()->getTimeZone());
    QJsonObject main;
    main.insert(QLatin1String("info"), info);
    QJsonArray array;
    for (auto fc : lc.forecasts()) {
        QJsonObject obj;
        obj.insert(QLatin1String("time"), fc->time().toString(Qt::ISODate));
        obj.insert(QLatin1String("weatherIcon"), fc->weatherIcon());
        obj.insert(QLatin1String("weatherDescription"), fc->weatherDescription());
        obj.insert(QLatin1String("maxTemp"), fc->maxTemp());
        obj.insert(QLatin1String("minTemp"), fc->minTemp());
        obj.insert(QLatin1String("windDirection"), fc->windDirection());
        obj.insert(QLatin1String("windSpeed"), fc->windSpeed());
        obj.insert(QLatin1String("precipitation"), fc->precipitation());
        obj.insert(QLatin1String("fog"), fc->fog());
        obj.insert(QLatin1String("cloudiness"), fc->cloudiness());
        obj.insert(QLatin1String("humidity"), fc->humidity());
        obj.insert(QLatin1String("pressure"), fc->pressure());
        array.push_back(obj);
    }
    main.insert(QLatin1String("main"), array);
    QJsonDocument doc;
    doc.setObject(main);
    return doc;
}

QList<WeatherLocation *> WeatherForecastManager::convertFromJson(QByteArray data)
{
    QList<AbstractWeatherForecast *> forecasts;
    QJsonDocument doc;
    doc.fromJson(data);
    auto api = new NMIWeatherAPI();
    api->setLocation(doc["info"]["latitude"].toDouble(), doc["info"]["longitude"].toDouble());
    api->setTimeZone(doc["info"]["timezone"].toString());
    auto location = new WeatherLocation(api, doc["info"]["name"].toString(), doc["info"]["latitude"].toDouble(), doc["info"]["longitude"].toDouble()); // one cache file corresponds to one location
    QJsonArray array = doc["main"].toArray();
    for (auto hour : array) {
        auto fc = new AbstractWeatherForecast();
        fc->setTime(QDateTime::fromString(hour.toObject()["time"].toString(), Qt::ISODate));
        fc->setWeatherIcon(hour.toObject()["weatherIcon"].toString());
        fc->setWeatherDescription(hour.toObject()["weatherDescription"].toString());
        fc->setMaxTemp(hour.toObject()["maxTemp"].toInt());
        fc->setMinTemp(hour.toObject()["minTemp"].toInt());
        fc->setWindDirection(hour.toObject()["windDirection"].toString());
        fc->setWindSpeed(hour.toObject()["windSpeed"].toInt());
        fc->setPrecipitation(hour.toObject()["precipitation"].toDouble());
        fc->setFog(hour.toObject()["fog"].toInt());
        fc->setCloudiness(hour.toObject()["cloudiness"].toInt());
        fc->setHumidity(hour.toObject()["humidity"].toInt());
        fc->setPressure(hour.toObject()["pressure"].toInt());
        forecasts.push_back(fc);
    }
    location->updateData(forecasts);
    return forecasts;
}
