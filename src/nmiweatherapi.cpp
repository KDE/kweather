#include "nmiweatherapi.h"
#include "geotimezone.h"
#include <QCoreApplication>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimeZone>
#include <QUrlQuery>
#include <QXmlStreamReader>
#include <zlib.h>
void NMIWeatherAPI::setLocation(float latitude, float longitude)
{
    lat = latitude;
    lon = longitude;
}

void NMIWeatherAPI::setToken(QString &)
{
} // no token is needed

NMIWeatherAPI::NMIWeatherAPI()
    : AbstractWeatherAPI(-1)
{
    connect(mManager, &QNetworkAccessManager::finished, this, &NMIWeatherAPI::parse);
}

NMIWeatherAPI::~NMIWeatherAPI()
{
    if (!tz) {
        delete tz;
    }
}

void NMIWeatherAPI::update()
{
    if (timeZone.isEmpty()) {
        tz = new GeoTimeZone(lat, lon);
        connect(tz, &GeoTimeZone::finished, this, &NMIWeatherAPI::setTZ); // if this failed, we will block forever, see line 106
    }                                                                     // TODO: fix this
    if (!mForecasts.empty()) {
        for (auto fc : mForecasts)
            delete fc;
        mForecasts.clear();
    } // delete old data

    // ported from itinerary/weather
    /*QUrl url;

    url.setScheme(QStringLiteral("https"));
    url.setHost(QStringLiteral("api.met.no"));
    url.setPath(QStringLiteral("/weatherapi/locationforecast/1.9/"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("lat"), QString::number(lat));
    query.addQueryItem(QStringLiteral("lon"), QString::number(lon));
    url.setQuery(query);

    qDebug() << url;
    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    // see §Identification on https://api.met.no/conditions_service.html
    req.setHeader(QNetworkRequest::UserAgentHeader, QString(QCoreApplication::applicationName() + QLatin1Char(' ') + QCoreApplication::applicationVersion() + QLatin1String(" (kde-pim@kde.org)")));

    // TODO see §Cache on https://api.met.no/conditions_service.html
    // see §Compression on https://api.met.no/conditions_service.html
    req.setRawHeader("Accept-Encoding", "gzip");
    mReply = mManager->get(req);*/
    parse(0);
}

void NMIWeatherAPI::parse(QNetworkReply *reply)
{
    qDebug() << "data arrived";

    QFile file;
    file.setFileName("data");
    file.open(QIODevice::ReadOnly);
    auto data = file.readAll();
    file.close();
    if ((data.size() < 4) || (data.at(0) != 0x1f) || (data.at(1) != char(0x8b))) {
        qWarning() << "Invalid gzip format";
        return;
    }
    QByteArray uncomp;
    QXmlStreamReader reader;
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
    reader.addData(uncomp);
    while (timeZone.isEmpty()) { // if we don't get timezone data, block
        qDebug() << "no timezone";
        continue;
    }
    xmlParse(reader, mForecasts);
    reply->deleteLater();
    delete mForecasts.back(); // tmp fix
    mForecasts.pop_back();
    emit updated(this->mForecasts);
}

void NMIWeatherAPI::xmlParse(QXmlStreamReader &reader, QList<AbstractWeatherForecast *> &list)
{
    if (list.isEmpty()) {
        auto fc = new AbstractWeatherForecast();
        list.push_back(fc);
    }
    while (!reader.atEnd()) {
        auto forecast = list.back();
        if (reader.readNextStartElement()) {
            if (reader.name() == QLatin1String("weatherdata") || reader.name() == QLatin1String("meta") || reader.name() == QLatin1String("product")) {
                continue;
            }
            if (reader.name() == QLatin1String("time")) {
                if (reader.attributes().value(QLatin1String("from")).toString() == reader.attributes().value(QLatin1String("to")).toString()) {
                    auto datetime = QDateTime::fromString(reader.attributes().value(QLatin1String("from")).toString(), Qt::ISODate);
                    datetime.setTimeZone(QTimeZone(QByteArray::fromStdString(timeZone.toStdString())));
                    datetime = datetime.toLocalTime();
                    forecast->setTime(datetime);

                    // resolve day or night weather icon (after forecast creation)
                    forecast->setNeutralWeatherIcon(apiDescMap[{forecast->weatherIcon().toInt(), WeatherDescId::WeatherType::NEUTRAL}].icon);
                    if (forecast->time().time().hour() >= 18 || forecast->time().time().hour() <= 6) { // TODO use system sunrise and sunset instead
                        forecast->setWeatherDescription(apiDescMap[{forecast->weatherIcon().toInt(), WeatherDescId::WeatherType::NIGHT}].desc);
                        forecast->setWeatherIcon(apiDescMap[{forecast->weatherIcon().toInt(), WeatherDescId::WeatherType::NIGHT}].icon);
                    } else {
                        forecast->setWeatherDescription(apiDescMap[{forecast->weatherIcon().toInt(), WeatherDescId::WeatherType::DAY}].desc);
                        forecast->setWeatherIcon(apiDescMap[{forecast->weatherIcon().toInt(), WeatherDescId::WeatherType::DAY}].icon);
                    }

                    list.push_back(new AbstractWeatherForecast());
                }
            }
            parseElement(reader, forecast);
        }
    }
}

void NMIWeatherAPI::parseElement(QXmlStreamReader &reader, AbstractWeatherForecast *fc)
{
    while (!reader.atEnd()) {
        switch (reader.tokenType()) {
        case QXmlStreamReader::StartElement:

            if (reader.name() == QLatin1String("temperature")) {
                const auto t = reader.attributes().value(QLatin1String("value")).toFloat();
                fc->setMinTemp(t);
                fc->setMaxTemp(t);
            } else if (reader.name() == QLatin1String("windDirection")) {
                fc->setWindDirection(reader.attributes().value(QLatin1String("name")).toString());
            } else if (reader.name() == QLatin1String("windSpeed")) {
                fc->setWindSpeed(reader.attributes().value(QLatin1String("mps")).toInt());
            } else if (reader.name() == QLatin1String("humidity")) {
                fc->setHumidity(reader.attributes().value(QLatin1String("value")).toInt());
            } else if (reader.name() == QLatin1String("pressure")) {
                fc->setPressure(reader.attributes().value(QLatin1String("value")).toInt());
            } else if (reader.name() == QLatin1String("cloudiness")) {
                fc->setCloudiness(reader.attributes().value(QLatin1String("percent")).toInt());
            } else if (reader.name() == QLatin1String("fog")) {
                fc->setFog(reader.attributes().value(QLatin1String("percent")).toFloat());
            } else if (reader.name() == QLatin1String("minTemperature")) {
                fc->setMinTemp(reader.attributes().value(QLatin1String("value")).toFloat());
            } else if (reader.name() == QLatin1String("maxTemperature")) {
                fc->setMaxTemp(reader.attributes().value(QLatin1String("value")).toFloat());
            } else if (reader.name() == QLatin1String("symbol")) {
                auto symId = reader.attributes().value(QLatin1String("number")).toInt();
                if (symId > 100)                            // https://api.met.no/weatherapi/weathericon/1.1/documentation
                    symId -= 100;                           // map polar night symbols
                fc->setWeatherIcon(QString::number(symId)); // set as id temporarily (time is not set yet, but we need it to determine day or night icon)
            } else if (reader.name() == QLatin1String("precipitation")) {
                fc->setPrecipitation(reader.attributes().value(QLatin1String("value")).toFloat());
            }
            break;

        case QXmlStreamReader::EndElement:

            if (reader.name() == QLatin1String("time")) {
                return;
            }
            break;

        default:
            break;
        }
        reader.readNext();
    }
}

void NMIWeatherAPI::setTZ()
{
    timeZone = tz->getTimeZone();
    qDebug() << "timezone" << timeZone;
    delete tz;
}
