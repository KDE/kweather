/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "geotimezone.h"
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
GeoTimeZone::GeoTimeZone(float lat, float lon, QObject *parent)
    : QObject(parent)
{
    manager = new QNetworkAccessManager();
    QUrl url;
    url.setScheme(QStringLiteral("http"));
    url.setHost(QStringLiteral("api.geonames.org"));
    url.setPath(QStringLiteral("/timezoneJSON"));
    QUrlQuery query;
    query.addQueryItem(QLatin1String("lat"), QString::number(lat));
    query.addQueryItem(QLatin1String("lng"), QString::number(lon));
    query.addQueryItem(QLatin1String("username"), QLatin1String("kweatherdev"));
    url.setQuery(query);
    qDebug() << url;
    QNetworkRequest req(url);
    manager->get(req);
    connect(manager, &QNetworkAccessManager::finished, this, &GeoTimeZone::downloadFinished);
}

void GeoTimeZone::downloadFinished(QNetworkReply *reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    // if our api calls reached daily limit
    if (doc[QLatin1String("status")][QLatin1String("value")].toInt() == 18) {
        qWarning() << "api calls reached daily limit";
        reply->deleteLater();
        return;
    }
    tz = doc["timezoneId"].toString();
    reply->deleteLater();
    emit finished();
}

QString GeoTimeZone::getTimeZone()
{
    return tz;
}

GeoTimeZone::~GeoTimeZone()
{
    delete manager;
}
