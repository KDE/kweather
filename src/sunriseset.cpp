/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "sunriseset.h"
#include "abstractsunrise.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
SunRiseSet::SunRiseSet(float latitude, float longitude, int offset_secs)
    : latitude_(latitude)
    , longitude_(longitude)
    , offset_(offset_secs)
{
    manager = new QNetworkAccessManager();
    connect(manager, &QNetworkAccessManager::finished, this, &SunRiseSet::process);
}

void SunRiseSet::update(int days)
{
    if (days > 15)
        days = 15;
    QUrl url;
    url.setScheme(QStringLiteral("https"));
    url.setHost(QStringLiteral("api.met.no"));
    url.setPath(QStringLiteral("/weatherapi/sunrise/2.0/.json"));
    QUrlQuery query;
    query.addQueryItem(QLatin1String("lat"), QString::number(latitude_));
    query.addQueryItem(QLatin1String("lon"), QString::number(longitude_));
    query.addQueryItem(QLatin1String("date"), QDate::currentDate().toString(QLatin1String("yyyy-MM-dd")));
    query.addQueryItem(QLatin1String("days"), QString::number(days));
    QString offset;
    if (offset < 0) {
        offset_ = -offset_;
        offset.append("-");
    }
    int hour = offset_ / 3600;
    if (hour >= 10)
        offset.append(QString::number(hour) + ":");
    else {
        offset.append("0" + QString::number(hour) + ":");
    }
    int min = (offset_ - hour * 3600) / 60;
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
}

//*~~~~~~~~ WIP ~~~~~~~~~*//
void SunRiseSet::process(QNetworkReply *reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonArray array = doc["location"].toObject()["time"].toArray();
    QJsonObject sunrise = array.at(0).toObject()["sunrise"].toObject();
    QJsonObject sunset = array.at(0).toObject()["sunset"].toObject();

    emit finished();
    reply->deleteLater();
}

SunRiseSet::~SunRiseSet()
{
    delete manager;
    for (auto sr : sunrise_)
        if (sr)
            delete sr;
}
