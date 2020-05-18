/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "geoiplookup.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>
GeoIPLookup::GeoIPLookup()
{
    mManager = new QNetworkAccessManager();
    QUrl url;
    url.setScheme(QStringLiteral("http"));
    url.setHost(QStringLiteral("geoip.ubuntu.com"));
    url.setPath(QStringLiteral("/lookup"));
    QNetworkRequest req(url);
    mReply = mManager->get(req);
    connect(mManager, &QNetworkAccessManager::finished, this, &GeoIPLookup::process);
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
void GeoIPLookup::process(QNetworkReply *reply)
{
    auto reader = new QXmlStreamReader(reply->readAll());

    while (!reader->atEnd()) {
        reader->readNext();
//        if (reader->name() == QLatin1String("CountryName")) old format: Country, City
//            locationName.append(reader->readElementText());
//        else if (reader->name() == QLatin1String("City"))
//            locationName.append(", " + reader->readElementText()); // <City>
        if (reader->name() == QLatin1String("City"))
            locationName.append(reader->readElementText());
        else if (reader->name() == QLatin1String("Latitude"))
            latitude_ = reader->readElementText().toFloat();
        else if (reader->name() == QLatin1String("Longitude"))
            longitude_ = reader->readElementText().toFloat();
        else if (reader->name() == "TimeZone")
            timeZone_ = reader->readElementText();
    }
    emit finished();
}

float GeoIPLookup::latitude()
{
    return latitude_;
}

float GeoIPLookup::longitude()
{
    return longitude_;
}

QString GeoIPLookup::name()
{
    return locationName;
}

QString GeoIPLookup::timeZone()
{
    return timeZone_;
}

GeoIPLookup::~GeoIPLookup()
{
    delete mManager;
}
