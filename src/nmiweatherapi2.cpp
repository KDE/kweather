#include "nmiweatherapi2.h"
#include "abstractdailyweatherforecast.h"
#include "abstractweatherforecast.h"
#include "abstracthourlyweatherforecast.h"
#include "geotimezone.h"

#include <QCoreApplication>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimeZone>
#include <QUrlQuery>
#include <QXmlStreamReader>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include <zlib.h>

void NMIWeatherAPI2::setLocation(float latitude, float longitude)
{
    lat = latitude;
    lon = longitude;
}

void NMIWeatherAPI2::setToken(QString &)
{
} // no token is needed

NMIWeatherAPI2::NMIWeatherAPI2()
    : AbstractWeatherAPI(-1)
{
    //    connect(mManager, &QNetworkAccessManager::finished, this, &NMIWeatherAPI::parse);
}

NMIWeatherAPI2::~NMIWeatherAPI2()
{
    if (!tz) {
        delete tz;
    }
}

void NMIWeatherAPI2::update()
{
    if (timeZone.isEmpty()) {
        tz = new GeoTimeZone(lat, lon);
        connect(tz, &GeoTimeZone::finished, this, &NMIWeatherAPI2::setTZ); // if this failed, we will block forever, see line 106
    }                                                                     // TODO: fix this

    QUrl url("https://api.met.no/weatherapi/locationforecast/2.0/");
    QUrlQuery query;
    query.addQueryItem("lat", QString::number(lat));
    query.addQueryItem("lon", QString::number(lon));

    url.setQuery(query);

    qDebug() << url;
    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    // see §Identification on https://api.met.no/conditions_service.html
    req.setHeader(QNetworkRequest::UserAgentHeader, QString(QCoreApplication::applicationName() + QLatin1Char(' ') + QCoreApplication::applicationVersion() + QLatin1String(" (kde-pim@kde.org)")));

    // TODO see §Cache on https://api.met.no/conditions_service.html
    // see §Compression on https://api.met.no/conditions_service.html
    req.setRawHeader("Accept-Encoding", "gzip");
    mReply = mManager->get(req);
    connect(
        mReply, &QNetworkReply::finished, this, [this]() { this->parse(this->mReply); }, Qt::ConnectionType::UniqueConnection);
}

void NMIWeatherAPI2::parse(QNetworkReply *reply)
{
    qDebug() << "data arrived";

    auto data = reply->readAll();
    if ((data.size() < 4) || (data.at(0) != 0x1f) || (data.at(1) != char(0x8b))) {
        qWarning() << "Invalid gzip format";
        return;
    }
    QByteArray uncomp;
    z_stream stream;
    unsigned char buffer[4096];

    stream.zalloc = nullptr;
    stream.zfree = nullptr;
    stream.opaque = nullptr;
    stream.avail_in = data.size();
    stream.next_in = reinterpret_cast<unsigned char *>(const_cast<char *>(data.data()));

    auto ret = inflateInit2(&stream, 15 + 32); // see docs, the magic numbers
    // enable gzip decoding

    if (ret != Z_OK) {
        qWarning() << "Failed to initialize zlib stream.";
        return;
    }

    do {
        stream.avail_out = sizeof(buffer);
        stream.next_out = buffer;

        ret = inflate(&stream, Z_NO_FLUSH);

        if ((ret != Z_OK) && (ret != Z_STREAM_END)) {
            qWarning() << "Zlib decoding failed!" << ret;
            break;
        }

        uncomp.append(reinterpret_cast<char *>(buffer), sizeof(buffer) - stream.avail_out);
    } while (stream.avail_out == 0);
    inflateEnd(&stream);

    qDebug() << "uncompressed";

    reply->deleteLater();

    // parse json
    QJsonDocument jsonDocument = QJsonDocument::fromJson(uncomp);
    if (jsonDocument.isObject()) {
        QJsonObject obj = jsonDocument.object();

        if (obj.contains("properties.timeseries")) {
            QJsonArray timeseries = obj.value("properties.timeseries").toArray();

            QHash<QDate, AbstractDailyWeatherForecast *> dayCache;
            QList<AbstractHourlyWeatherForecast*> hoursList;

            // loop over all forecast data
            for (QJsonValueRef ref : timeseries) {
                QJsonObject refObj = ref.toObject();
                parseOneElement(refObj, dayCache, hoursList);
            }

            // delete old data
            delete currentData_;
            // process and build abstract forecast
            currentData_ = new AbstractWeatherForecast("TODO Weather Location", lat, lon, hoursList, dayCache.values());

            // TODO set location currentData_.setLocation
        }
    }

    // TODO: emit AbstractForecast
    emit updated(this->currentData());
}

void NMIWeatherAPI2::parseOneElement(QJsonObject &object, QHash<QDate, AbstractDailyWeatherForecast *> &dayCache, QList<AbstractHourlyWeatherForecast *> &hoursList)
{
    if (!object.contains("data.instant.details") || (!object.contains("data.next_6_hours") && !object.contains("data.next_1_hours"))) return;

    auto date = QDateTime::fromString(object.value("time").toString(), Qt::ISODate);
    auto* hourForecast = new AbstractHourlyWeatherForecast();

    // set initial hour fields
    hourForecast->setDate(date); // the first time will be at the exact time of query, otherwise the beginning of each hour
    hourForecast->setTemperature(object.value("data.instant.air_temperature").toDouble());
    hourForecast->setPressure(object.value("data.instant.air_pressure_at_sea_level").toDouble());
    // TODO wind direction
    hourForecast->setWindSpeed(object.value("data.instant.wind_speed").toDouble());
    hourForecast->setHumidity(object.value("data.instant.relative_humidity").toDouble());
    hourForecast->setFog(object.value("data.instant.fog_area_fraction").toDouble());
    hourForecast->setUvIndex(object.value("data.instant.ultraviolet_index_clear_sky").toDouble());

    QString symbolCode;
    // some fields contain only "next_1_hours", and others may contain only "next_6_hours"
    if (object.contains("data.next_1_hours")) {
        symbolCode = object.value("data.next_1_hours.summary.symbol_code").toString("unknown");
        hourForecast->setPrecipitationAmount(object.value("data.next_1_hours.precipitation_amount").toDouble());
    } else {
        symbolCode = object.value("data.next_6_hours.summary.symbol_code").toString("unknown");
        hourForecast->setPrecipitationAmount(object.value("data.next_6_hours.precipitation_amount").toDouble());
    }

    symbolCode = symbolCode.split('_')[0]; // trim _[day/night] from end - https://api.met.no/weatherapi/weathericon/2.0/legends
    hourForecast->setNeutralWeatherIcon(apiDescMap[symbolCode + "_neutral"].icon);

    // figure out whether to use night or day weather icon and description
    if (hourForecast->date().time().hour() >= 18 || hourForecast->date().time().hour() <= 6) { // TODO use system sunrise and sunset instead
        hourForecast->setWeatherDescription(apiDescMap[symbolCode + "_night"].desc);
        hourForecast->setWeatherIcon(apiDescMap[symbolCode + "_night"].icon);
    } else {
        hourForecast->setWeatherDescription(apiDescMap[symbolCode + "_day"].desc);
        hourForecast->setWeatherIcon(apiDescMap[symbolCode + "_day"].icon);
    }

    // add day if not already created
    if (!dayCache.contains(date.date())) {
        dayCache[date.date()] = new AbstractDailyWeatherForecast(-1e9, 1e9, 0, "weather-none-available", "", date.date());
    }

    // update day forecast with hour information if needed
    AbstractDailyWeatherForecast* dayForecast = dayCache[date.date()];
    dayForecast->setMaxTemp(std::max(dayForecast->maxTemp(), (float) object.value("data.next_6_hours.details.air_temperature_max").toDouble()));
    dayForecast->setMinTemp(std::min(dayForecast->minTemp(), (float) object.value("data.next_6_hours.details.air_temperature_min").toDouble()));

    if (object.contains("data.next_1_hours")) {
        dayForecast->setPrecipitation(dayForecast->precipitation() + object.value("data.next_1_hours.precipitation_amount").toDouble());
    } else {
        dayForecast->setPrecipitation(dayForecast->precipitation() + object.value("data.next_6_hours.precipitation_amount").toDouble());
    }

    // rank weather (for what best describes the day overall)
    QHash<QString, int> rank = {// only need neutral icons
        {"weather-none-available", 0},
        {"weather-clear", 0},
        {"weather-few-clouds", 1},
        {"weather-clouds", 2},
        {"weather-fog", 3},
        {"weather-mist", 3},
        {"weather-showers-scattered", 4},
        {"weather-snow-scattered", 4},
        {"weather-showers", 5},
        {"weather-hail", 5},
        {"weather-snow", 5},
        {"weather-freezing-rain", 6},
        {"weather-freezing-storm", 6},
        {"weather-snow-rain", 6},
        {"weather-storm", 7}
    };

    // set description and icon if it is higher ranked
    if (rank[hourForecast->weatherIcon()] > rank[dayForecast->weatherIcon()]) {
        dayForecast->setWeatherDescription(hourForecast->weatherDescription());
        dayForecast->setWeatherIcon(hourForecast->weatherIcon());
    }

    // add hour forecast to list
    hoursList.append(hourForecast);
}

void NMIWeatherAPI2::setTZ()
{
    timeZone = tz->getTimeZone();
    qDebug() << "timezone" << timeZone;
    delete tz;
}
