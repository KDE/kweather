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

void WeatherForecastManager::writeToCache(WeatherLocation &data)
{
    QFile file;
    QString url;
    url = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir dir(url.append(QString("/cache/%1/%2/").arg(QString::number(static_cast<int>(data.latitude() * 100))).arg(QString::number(static_cast<int>(data.longitude() * 100))))); // create cache location
    if (!dir.exists())
        dir.mkpath(".");
    // should be this path: /home/user/.cache/kweather/7000/3000 for location with coordinate 70.00 30.00
    file.setFileName(dir.path() + "/cache.json");
    qDebug() << file.fileName();
    file.open(QIODevice::WriteOnly);
    file.write(convertToJson(data).toJson(QJsonDocument::Compact)); // write json
    file.close();
}

// ##### Work In Progress #####

void WeatherForecastManager::readFromCache()
{
    int i = model_.getList().count(); // index for weatherlistmodel
    QFile reader;
    QDirIterator It(QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/cache", QDirIterator::Subdirectories); // list directory entries
    while (It.hasNext()) {
        reader.setFileName(It.next());
        qDebug() << reader.fileName();
        if (reader.fileName().right(10) != QLatin1String("cache.json")) // if not cache file ignore
            continue;
        reader.open(QIODevice::ReadOnly | QIODevice::Text);
        model_.insert(i, convertFromJson(reader.readAll())); // insert location into model
        reader.close();
        i++; // add one location count
    }
}

QJsonDocument WeatherForecastManager::convertToJson(WeatherLocation &lc) // Qt uses QByteArray internally, pass by copy
{
    QJsonObject info;
    info[QLatin1String("name")] = lc.locationName();
    info[QLatin1String("latitude")] = QString::number(lc.latitude());
    info[QLatin1String("longitude")] = QString::number(lc.longitude());
    info[QLatin1String("timezone")] = lc.weatherBackendProvider()->getTimeZone();
    QJsonObject main;
    main[QLatin1String("info")] = info;
    QJsonArray hourArray;
    QJsonArray dayArray;
    for (auto fc : lc.forecast()->hourlyForecasts()) {
        QJsonObject obj;
        obj[QLatin1String("time")] = fc->date().toSecsSinceEpoch();
        obj[QLatin1String("weatherIcon")] = fc->weatherIcon();
        obj[QLatin1String("weatherDescription")] = fc->weatherDescription();
        obj[QLatin1String("temperature")] = fc->temperature();
        obj[QLatin1String("windDirection")] = static_cast<int>(fc->windDirection());
        obj[QLatin1String("windSpeed")] = fc->windSpeed();
        obj[QLatin1String("precipitation")] = fc->precipitationAmount();
        obj[QLatin1String("fog")] = fc->fog();
        obj[QLatin1String("uvIndex")] = fc->uvIndex();
        obj[QLatin1String("humidity")] = fc->humidity();
        obj[QLatin1String("pressure")] = fc->pressure();
        hourArray.push_back(obj);
    }

    for (auto fc : lc.forecast()->dailyForecasts()) {
        QJsonObject obj;
        obj[QLatin1String("time")] = fc->date().toString(Qt::ISODate);
        obj[QLatin1String("weatherIcon")] = fc->weatherIcon();
        obj[QLatin1String("weatherDescription")] = fc->weatherDescription();
        obj[QLatin1String("maxTemp")] = fc->maxTemp();
        obj[QLatin1String("minTemp")] = fc->minTemp();
        obj[QLatin1String("precipitation")] = fc->precipitation();
        dayArray.push_back(obj);
    }
    QJsonObject data;
    data[QLatin1String("hour")] = hourArray;
    data[QLatin1String("day")] = dayArray;
    main[QLatin1String("main")] = data;
    QJsonDocument doc;
    doc.setObject(main);
    return doc;
}

WeatherLocation *WeatherForecastManager::convertFromJson(QByteArray data)
{
    auto now = QDateTime::currentDateTime();
    auto forecast = new AbstractWeatherForecast();
    QList<AbstractHourlyWeatherForecast *> hourlyForecast;
    QList<AbstractDailyWeatherForecast *> dailyForecast;
    QJsonObject doc = QJsonDocument::fromJson(data).object();

    QJsonArray hourArray = doc[QLatin1String("main")].toObject()[QLatin1String("hour")].toArray();
    QJsonArray dayArray = doc[QLatin1String("main")].toObject()[QLatin1String("day")].toArray();

    forecast->setLatitude(doc[QLatin1String("info")].toObject()[QLatin1String("latitude")].toDouble());
    forecast->setLongitude(doc[QLatin1String("info")].toObject()[QLatin1String("longitude")].toDouble());
    forecast->setLocationName(doc[QLatin1String("info")].toObject()[QLatin1String("name")].toString());
    forecast->setTimeCreated(QDateTime::fromSecsSinceEpoch(hourArray.at(0).toObject()["time"].toInt()));

    auto api = new NMIWeatherAPI2();
    api->setTimeZone(doc[QLatin1String("info")].toObject()[QLatin1String("timezone")].toString());
    api->setLocation(doc[QLatin1String("info")].toObject()[QLatin1String("latitude")].toDouble(), doc[QLatin1String("info")].toObject()[QLatin1String("longitude")].toDouble());

    auto location = new WeatherLocation(api,
                                        doc[QLatin1String("info")].toObject()[QLatin1String("name")].toString(),
                                        doc[QLatin1String("info")].toObject()[QLatin1String("latitude")].toDouble(),
                                        doc[QLatin1String("info")].toObject()[QLatin1String("longitude")].toDouble());

    for (auto hour : hourArray) {
        auto date = QDateTime::fromSecsSinceEpoch(hour.toObject()["time"].toInt());
        if (date.secsTo(now) > 3600) // if not fresh, throw away
            continue;
        auto fc = new AbstractHourlyWeatherForecast();
        fc->setDate(date);
        fc->setWeatherIcon(hour.toObject()["weatherIcon"].toString());
        fc->setWeatherDescription(hour.toObject()["weatherDescription"].toString());
        fc->setTemperature(hour.toObject()["temperature"].toDouble());
        fc->setUvIndex(hour.toObject()["uvIndex"].toDouble());
        fc->setWindDirection(static_cast<AbstractHourlyWeatherForecast::WindDirection>(hour.toObject()["windDirection"].toInt()));
        fc->setWindSpeed(hour.toObject()["windSpeed"].toDouble());
        fc->setPrecipitationAmount(hour.toObject()["precipitation"].toDouble());
        fc->setFog(hour.toObject()["fog"].toDouble());
        fc->setHumidity(hour.toObject()["humidity"].toDouble());
        fc->setPressure(hour.toObject()["pressure"].toDouble());
        hourlyForecast.push_back(fc);
    }

    for (auto day : dayArray) {
        auto date = QDate::fromString(day.toObject()["time"].toString(), Qt::ISODate);
        if (date.daysTo(now.date()) > 0)
            continue;
        auto fc = new AbstractDailyWeatherForecast();
        fc->setDate(date);
        fc->setWeatherIcon(day.toObject()["weatherIcon"].toString());
        fc->setWeatherDescription(day.toObject()["weatherDescription"].toString());
        fc->setMaxTemp(day.toObject()["maxTemp"].toDouble());
        fc->setMinTemp(day.toObject()["minTemp"].toDouble());
        fc->setPrecipitation(day.toObject()["precipitation"].toDouble());
        dailyForecast.push_back(fc);
    }

    // convert time to corresponding localtime

    for (auto fc : hourlyForecast) {
        fc->setDate(fc->date().toTimeZone(QTimeZone(QByteArray::fromStdString(doc[QLatin1String("info")].toObject()[QLatin1String("timezone")].toString().toStdString()))));
        qDebug() << "fc time" << fc->date().toString();
    }

    forecast->setDailyForecasts(dailyForecast);
    forecast->setHourlyForecasts(hourlyForecast);
    location->updateData(forecast);
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
