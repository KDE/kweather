#include "norwegianmeteorologicalinstitute.h"

#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCoreApplication>
#include <QNetworkRequest>
#include <QXmlStreamReader>

#include <zlib.h>

void NorwegianMeteorologicalInstitute::setLocation(float latitude,
                                                   float longitude) {
  lat = latitude;
  lon = longitude;
}

void NorwegianMeteorologicalInstitute::setToken(QString&) {} // no token is

// needed

NorwegianMeteorologicalInstitute::NorwegianMeteorologicalInstitute()
  : AbstractAPI(-1)
{}

void NorwegianMeteorologicalInstitute::update() {
  if (!mForecasts.empty()) {
    for (auto fc : mForecasts) delete fc;
    mForecasts.clear();
  } // delete old data

  // ported from itinerary/weather
  QUrl url;

  url.setScheme(QStringLiteral("https"));
  url.setHost(QStringLiteral("api.met.no"));
  url.setPath(QStringLiteral("/weatherapi/locationforecast/1.9/"));
  QUrlQuery query;
  query.addQueryItem(QStringLiteral("lat"), QString::number(lat));
  query.addQueryItem(QStringLiteral("lon"), QString::number(lon));
  url.setQuery(query);

  qDebug() << url;
  QNetworkRequest req(url);
  req.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                   QNetworkRequest::NoLessSafeRedirectPolicy);

  // see §Identification on https://api.met.no/conditions_service.html
  req.setHeader(QNetworkRequest::UserAgentHeader,
                QString(QCoreApplication::applicationName() +
                        QLatin1Char(' ') +
                        QCoreApplication::applicationVersion() +
                        QLatin1String(" (kde-pim@kde.org)")));

  // TODO see §Cache on https://api.met.no/conditions_service.html
  // see §Compression on https://api.met.no/conditions_service.html
  req.setRawHeader("Accept-Encoding", "gzip");
  mReply = mManager->get(req);
  connect(mManager, &QNetworkAccessManager::finished, this,
          &NorwegianMeteorologicalInstitute::parse);
}

void NorwegianMeteorologicalInstitute::parse(QNetworkReply *reply) {
  // ported from itinerary, but directly parse it instead of caching
  const auto data = reply->readAll();

  if ((data.size() < 4) || (data.at(0) != 0x1f) || (data.at(1) != char(0x8b))) {
    qWarning() << "Invalid gzip format";
    return;
  }
  QXmlStreamReader reader;
  z_stream stream;
  unsigned char buffer[1024];

  stream.zalloc   = nullptr;
  stream.zfree    = nullptr;
  stream.opaque   = nullptr;
  stream.avail_in = data.size();
  stream.next_in  =
    reinterpret_cast<unsigned char *>(const_cast<char *>(data.data()));

  auto ret = inflateInit2(&stream, 15 + 32); // see docs, the magic numbers
                                             // enable gzip decoding

  if (ret != Z_OK) {
    qWarning() << "Failed to initialize zlib stream.";
    return;
  }

  do {
    stream.avail_out = sizeof(buffer);
    stream.next_out  = buffer;

    ret = inflate(&stream, Z_NO_FLUSH);

    if ((ret != Z_OK) && (ret != Z_STREAM_END)) {
      qWarning() << "Zlib decoding failed!" << ret;
      break;
    }

    reader.addData(QByteArray(reinterpret_cast<char *>(buffer),
                              sizeof(buffer) - stream.avail_out));
    xmlParse(reader, mForecasts);
  } while (stream.avail_out == 0);
  inflateEnd(&stream);
  emit updated();
}

void NorwegianMeteorologicalInstitute::xmlParse(
  QXmlStreamReader                      & reader,
  std::vector<AbstractWeatherForecast *>& vector)
{
  if (vector.empty()) {
    AbstractWeatherForecast fc;
    vector.push_back(&fc);
  }
  auto forecast = vector.back();

  while (!reader.atEnd()) {
    if (reader.tokenType() == QXmlStreamReader::StartElement) {
      if ((reader.name() == QLatin1String("weatherdata")) ||
          (reader.name() == QLatin1String("product"))) {
        reader.readNext(); // enter these elements
        continue;
      }

      if ((reader.name() == QLatin1String("time")) &&
          (reader.attributes().value(QLatin1String("datatype")) ==
           QLatin1String("forecast"))) {
        auto from =
          QDateTime::fromString(reader.attributes().value(QLatin1String(
                                                            "from")).toString(),
                                Qt::ISODate);
        auto to =
          QDateTime::fromString(reader.attributes().value(QLatin1String(
                                                            "to")).toString(),
                                Qt::ISODate);
        bool isFinished;

        if (from.time().hour() == to.time().hour()) forecast->setTime(from);  //
                                                                              //
                                                                              //
                                                                              //
                                                                              //
                                                                              //
                                                                              //
                                                                              // set
                                                                              //
                                                                              //
                                                                              //
                                                                              //
                                                                              //
                                                                              //
                                                                              // the
                                                                              //
                                                                              //
                                                                              //
                                                                              //
                                                                              //
                                                                              //
                                                                              // time

        isFinished = parseElement(reader, forecast);

        if (isFinished) //  if All data are read, add a blank one in the back
          vector.push_back(new AbstractWeatherForecast());

        // warning:
        // UTC
        // time
        // todo: calculate local time from UTC according to coordinate
      }
      reader.skipCurrentElement();
    }
    else {
      reader.readNext();
    }
  }
}

bool NorwegianMeteorologicalInstitute::parseElement(
  QXmlStreamReader       & reader,
  AbstractWeatherForecast *fc) {
  while (!reader.atEnd()) {
    switch (reader.tokenType()) {
    case QXmlStreamReader::StartElement:

      if (reader.name() == QLatin1String("temperature")) {
        const auto t =
          reader.attributes().value(QLatin1String("value")).toFloat();
        fc->setMinTemp(t);
        fc->setMaxTemp(t);
      } else if (reader.name() == QLatin1String("windDirection")) {
        fc->setWindDirection(reader.attributes().value(QLatin1String(
                                                         "name")).toString());
      } else if (reader.name() == QLatin1String("windSpeed")) {
        fc->setWindSpeed(reader.attributes().value(QLatin1String(
                                                     "mps")).toInt());
      }
      else if (reader.name() == QLatin1String("humidity")) {
        fc->setHumidity(reader.attributes().value(QLatin1String(
                                                    "value")).toInt());
      }
      else if (reader.name() == QLatin1String("pressure")) {
        fc->setPressure(reader.attributes().value(QLatin1String(
                                                    "value")).toInt());
      }
      else if (reader.name() == QLatin1String("cloudiness")) {
        fc->setCloudiness(reader.attributes().value(QLatin1String(
                                                      "percent")).toInt());
      }
      else if (reader.name() == QLatin1String("fog")) {
        fc->setFog(reader.attributes().value(QLatin1String(
                                               "percent")).toFloat());
      }
      else if (reader.name() == QLatin1String("minTemperature")) {
        fc->setMinTemp(reader.attributes().value(QLatin1String("value")).toFloat());
      } else if (reader.name() == QLatin1String("maxTemperature")) {
        fc->setMaxTemp(reader.attributes().value(QLatin1String("value")).toFloat());
      }
      else if (reader.name() == QLatin1String("symbol")) {
        auto symId = reader.attributes().value(QLatin1String("number")).toInt();

        if (symId > 100) {
          symId -= 100; // map polar night symbols
        }
        fc->setWeatherIcon(map.value(symId));
      }
      else if (reader.name() == QLatin1String("precipitation")) {
        fc->setPrecipitation(reader.attributes().value(QLatin1String(
                                                         "value")).toFloat());
      }
      break;

    case QXmlStreamReader::EndElement:

      if (reader.name() == QLatin1String("time")) {
        return true;
      }
      break;

    default:
      break;
    }
    reader.readNext();
  }
  return false;
}
