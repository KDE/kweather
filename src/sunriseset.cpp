/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "sunriseset.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
SunRiseSet::SunRiseSet(float latitude, float longitude, int offset_secs)
{
    manager = new QNetworkAccessManager();
    QUrl url;
    url.setScheme(QStringLiteral("https"));
    url.setHost(QStringLiteral("api.met.no"));
    url.setPath(QStringLiteral("/weatherapi/sunrise/2.0/.json"));
    QUrlQuery query;
    query.addQueryItem(QLatin1String("lat"), QString::number(latitude));
    query.addQueryItem(QLatin1String("lon"), QString::number(longitude));
    query.addQueryItem(QLatin1String("date"), QDate::currentDate().toString(QLatin1String("yyyy-MM-dd")));
    QString offset;
    if (offset_secs < 0) {
        offset_secs = -offset_secs;
        offset.append("-");
    }
    int hour = offset_secs / 3600;
    if (hour >= 10)
        offset.append(QString::number(hour) + ":");
    else {
        offset.append("0" + QString::number(hour) + ":");
    }
    int min = (offset_secs - hour * 3600) / 60;
    if (min >= 10) {
        offset.append(QString::number(min));
    } else {
        offset.append("0" + QString::number(min));
    }
    query.addQueryItem(QLatin1String("offset"), offset);
    url.setQuery(query);
    qDebug() << url;
    QNetworkRequest req(url);
    manager->get(req);
    connect(manager, &QNetworkAccessManager::finished, this, &SunRiseSet::process);
}

void SunRiseSet::process(QNetworkReply *reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonArray array = doc["location"].toObject()["time"].toArray();
    QJsonObject sunrise = array.at(0).toObject()["sunrise"].toObject();
    QJsonObject sunset = array.at(0).toObject()["sunset"].toObject();
    sunSet_ = sunset["time"].toString().left(13).right(2).toInt();
    sunRise_ = sunrise["time"].toString().left(13).right(2).toInt();

    emit finished();
    reply->deleteLater();
}

SunRiseSet::~SunRiseSet()
{
    delete manager;
}

int SunRiseSet::sunSet()
{
    return sunSet_;
}

int SunRiseSet::sunRise()
{
    return sunRise_;
}
