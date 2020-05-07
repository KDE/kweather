#include "weatherforecastmanager.h"
#include "nmiweatherapi2.h"
#include "owmweatherapi.h"
#include "weatherlocationmodel.h"
#include <QDebug>
#include <QDirIterator>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
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

    // create cache location if it does not exist, and load cache
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    if (!dir.exists())
        dir.mkpath(".");
    readFromCache();

    distribution = new std::uniform_int_distribution<int>(0, 30 * 60); // uniform random update interval, 60 min to 90 min
    auto rand = std::bind(*distribution, generator);
    cacheTimer = new QTimer(this);
    cacheTimer->start(1000 * 3600 * 3); // cache every three hours
    connect(cacheTimer, &QTimer::timeout, this, &WeatherForecastManager::cache);

    updateTimer = new QTimer(this);
    updateTimer->setSingleShot(true);
    connect(updateTimer, &QTimer::timeout, this, &WeatherForecastManager::update);
    if (api_ == NORWEGIAN)
        updateTimer->start(5000 /*1000 * 3600 + rand() * 1000*/);
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
}

void WeatherForecastManager::writeToCache(WeatherLocation &data)
{
    QFile file;
    QString url;
    url = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir dir(url.append(QString("/%1/%2/").arg(QString::number(static_cast<int>(data.latitude() * 100))).arg(QString::number(static_cast<int>(data.longitude() * 100))))); // create cache location
    if (!dir.exists())
        dir.mkpath(".");
    // should be this path: /home/user/.cache/kweather/7000/3000 for location with coordinate 70.00 30.00
    qDebug() << url;
    // TODO
//    file.setFileName(QString(url + QString::number(data.forecasts().back()->time().toSecsSinceEpoch()))); // file name is last forecast's unix time
    file.open(QIODevice::WriteOnly);                                                                      // this will later be used to determine if we want this cache or not
    file.write(convertToJson(data).toJson(QJsonDocument::Compact));                                       // write json
    file.close();
}

// ##### Work In Progress #####

void WeatherForecastManager::readFromCache()
{
    int i = 0;           // index for weatherlistmodel
    QVector<int> vector; // sort out cache freshness
    QFile reader;
    QDirIterator LatIt(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + QLatin1String("/kweather"), QDir::NoDotAndDotDot); // list directory entries
    while (LatIt.hasNext()) {                                                                                                               // list all longitude
        QDirIterator LonIt(LatIt.next(), QDir::Files);
        while (LonIt.hasNext()) {
            // This is for each individual location
            reader.setFileName(LonIt.next());
            qDebug() << "file name" << reader.fileName();
            if (reader.fileName().toLong() <= QDateTime::currentSecsSinceEpoch()) {
                reader.remove();
                continue;
            }
            vector.push_back(reader.fileName().toInt());
        }
        std::sort(vector.begin(), vector.end()); // ascending
        reader.setFileName(QString::number(*vector.end()));
        reader.open(QIODevice::ReadOnly);
        model_.insert(i, convertFromJson(reader.readAll())); // insert location into model
        reader.close();
        for (int j = 0; j < vector.count() - 1; j++) {
            reader.setFileName(QString::number(vector.at(j)));
            reader.remove(); // remove all not the freshest cache
        }
        vector.clear();
        i++; // add one location count
    }
}

QJsonDocument WeatherForecastManager::convertToJson(WeatherLocation &lc) // Qt uses QByteArray internally, pass by copy
{
    QJsonObject info;
    info[QLatin1String("name")] = lc.locationName();
    info[QLatin1String("latitude")] = QString::number(lc.latitude());
    info[QLatin1String("longitude")] = QString::number(lc.longitude());
//    info[QLatin1String("timezone")] = lc.weatherBackendProvider()->getTimeZone(); TODO
    QJsonObject main;
    main[QLatin1String("info")] = info;
    QJsonArray array;
    // TODO
//    for (auto fc : lc.forecasts()) {
//        QJsonObject obj;
//        obj[QLatin1String("time")] = fc->time().toString(Qt::ISODate);
//        obj[QLatin1String("weatherIcon")] = fc->weatherIcon();
//        obj[QLatin1String("weatherDescription")] = fc->weatherDescription();
//        obj[QLatin1String("maxTemp")] = QString::number(fc->maxTemp());
//        obj[QLatin1String("minTemp")] = QString::number(fc->minTemp());
//        obj[QLatin1String("windDirection")] = fc->windDirection();
//        obj[QLatin1String("windSpeed")] = QString::number(fc->windSpeed());
//        obj[QLatin1String("precipitation")] = QString::number(fc->precipitation());
//        obj[QLatin1String("fog")] = QString::number(fc->fog());
//        obj[QLatin1String("cloudiness")] = QString::number(fc->cloudiness());
//        obj[QLatin1String("humidity")] = QString::number(fc->humidity());
//        obj[QLatin1String("pressure")] = QString::number(fc->pressure());
//        array.push_back(obj);
//    }
    main[QLatin1String("main")] = array;
    QJsonDocument doc;
    doc.setObject(main);
    return doc;
}

WeatherLocation *WeatherForecastManager::convertFromJson(QByteArray data)
{
    QList<AbstractWeatherForecast *> forecasts;
    QJsonDocument doc;
    doc.fromJson(data);
    auto api = new NMIWeatherAPI2();
    api->setLocation(doc["info"]["latitude"].toDouble(), doc["info"]["longitude"].toDouble());
    api->setTimeZone(doc["info"]["timezone"].toString());
    auto location = new WeatherLocation(api, doc["info"]["name"].toString(), doc["info"]["latitude"].toDouble(), doc["info"]["longitude"].toDouble()); // one cache file corresponds to one location
    QJsonArray array = doc["main"].toArray();
    // TODO
//    for (auto hour : array) {
//        auto fc = new AbstractWeatherForecast();
//        fc->setTime(QDateTime::fromString(hour.toObject()["time"].toString(), Qt::ISODate));
//        fc->setWeatherIcon(hour.toObject()["weatherIcon"].toString());
//        fc->setWeatherDescription(hour.toObject()["weatherDescription"].toString());
//        fc->setMaxTemp(hour.toObject()["maxTemp"].toInt());
//        fc->setMinTemp(hour.toObject()["minTemp"].toInt());
//        fc->setWindDirection(hour.toObject()["windDirection"].toString());
//        fc->setWindSpeed(hour.toObject()["windSpeed"].toInt());
//        fc->setPrecipitation(hour.toObject()["precipitation"].toDouble());
//        fc->setFog(hour.toObject()["fog"].toInt());
//        fc->setCloudiness(hour.toObject()["cloudiness"].toInt());
//        fc->setHumidity(hour.toObject()["humidity"].toInt());
//        fc->setPressure(hour.toObject()["pressure"].toInt());
//        forecasts.push_back(fc);
//    }
//    location->updateData(forecasts);
    return location;
}

void WeatherForecastManager::cache()
{
    qDebug() << "start caching";
    auto list = model_.getList();
    if (list.isEmpty())
        return;
    for (auto lc : model_.getList()) {
        writeToCache(*lc);
    }
}
