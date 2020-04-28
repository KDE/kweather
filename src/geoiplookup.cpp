#include "geoiplookup.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>
geoiplookup::geoiplookup()
{
  mManager = new QNetworkAccessManager();
  QUrl url;
  url.setScheme(QStringLiteral("http"));
  url.setHost(QStringLiteral("geoip.ubuntu.com"));
  url.setPath(QStringLiteral("/lookup"));
  QNetworkRequest req(url);
  mReply = mManager->get(req);
  connect(mManager, &QNetworkAccessManager::finished, this,
          &geoiplookup::process);
}

/*
 * Response example:
   <Response>
    <Ip>***.***.***.***</Ip>
    <Status>OK</Status>
    <CountryCode>FR</CountryCode>
    <CountryCode3>FRA</CountryCode3>
    <CountryName>France</CountryName>
    <RegionCode>A8</RegionCode>
    <RegionName>Ile-de-France</RegionName>
    <City>Paris</City>
    <ZipPostalCode>75001</ZipPostalCode>
    <Latitude>48.8607</Latitude>
    <Longitude>2.3281</Longitude>
    <AreaCode>0</AreaCode>
    <TimeZone>Europe/Paris</TimeZone>
   </Response>
 */
void geoiplookup::process(QNetworkReply *reply) {
  auto reader = new QXmlStreamReader(reply->readAll());

  while (!reader->atEnd()) {
    reader->readNext();

    if (reader->name() == QLatin1String("CountryName")) {
      locationName.append(reader->readElementText());
      reader->readNext();
      reader->readNext();                                    // to <RegionName>
      locationName.append(", " + reader->readElementText());
      reader->readNext();
      locationName.append(", " + reader->readElementText()); // <City>
      reader->readNext();
      reader->readNext();                                    // to <Latitude>
      Lat = reader->readElementText().toFloat();
      reader->readNext();
      Lon = reader->readElementText().toFloat();
      break;
    }
  }
  emit finished();
}

float geoiplookup::lat() {
  return Lat;
}

float geoiplookup::lon() {
  return Lon;
}

QString geoiplookup::name() {
  return locationName;
}

geoiplookup::~geoiplookup() {
  delete mManager;
}
