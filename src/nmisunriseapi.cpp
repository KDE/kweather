/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "nmisunriseapi.h"
#include "abstractsunrise.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QtMath>
#include <QTimeZone>

NMISunriseAPI::NMISunriseAPI(float latitude, float longitude, int offset_secs)
    : latitude_(latitude)
    , longitude_(longitude)
    , offset_(offset_secs)
{
    manager = new QNetworkAccessManager(this);

    manager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
    manager->setStrictTransportSecurityEnabled(true);
    manager->enableStrictTransportSecurityStore(true);

    connect(manager, &QNetworkAccessManager::finished, this, &NMISunriseAPI::process);
}

void NMISunriseAPI::update()
{
    if (sunrise_.count() >= 10) // don't update if we have enough data
        return;
    QUrl url(QStringLiteral("https://api.met.no/weatherapi/sunrise/2.0/.json"));
    QUrlQuery query;
    query.addQueryItem(QLatin1String("lat"), QString::number(latitude_));
    query.addQueryItem(QLatin1String("lon"), QString::number(longitude_));
    // if we already have data, request data beyond the last day
    query.addQueryItem(QLatin1String("date"), sunrise_.isEmpty() ? QDate::currentDate().toString(QLatin1String("yyyy-MM-dd")) : QDate::currentDate().addDays(sunrise_.count()).toString(QLatin1String("yyyy-MM-dd")));
    query.addQueryItem(QLatin1String("days"), sunrise_.isEmpty() ? QString::number(10) : QString::number(11 - sunrise_.count()));

    // calculate offset (form example: -04:00)
    QString offset = offset_ < 0 ? "-" : "+";
    int hour = qFabs(offset_) / 3600;
    if (hour >= 10)
        offset.append(QString::number(hour) + ":");
    else {
        offset.append("0" + QString::number(hour) + ":");
    }
    int min = (qFabs(offset_) - hour * 3600) / 60;
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

void NMISunriseAPI::process(QNetworkReply *reply)
{
    reply->deleteLater();
    if (reply->error()) {
        qDebug() << "nmisunriseapi network error:" << reply->errorString();
        emit networkError();
        return;
    }
    
    QTimeZone tz = QTimeZone(offset_);

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonArray array = doc["location"].toObject()["time"].toArray();
    for (int i = 0; i <= array.count() - 2; i++) // we don't want last one
    {
        AbstractSunrise sr;
        sr.setSunSet(QDateTime::fromString(array.at(i).toObject()["sunset"].toObject()["time"].toString().left(19), "yyyy-MM-ddThh:mm:ss"));
        sr.setSunRise(QDateTime::fromString(array.at(i).toObject()["sunrise"].toObject()["time"].toString().left(19), "yyyy-MM-ddThh:mm:ss"));
        sr.setMoonSet(QDateTime::fromString(array.at(i).toObject()["moonset"].toObject()["time"].toString().left(19), "yyyy-MM-ddThh:mm:ss"));
        sr.setMoonRise(QDateTime::fromString(array.at(i).toObject()["moonrise"].toObject()["time"].toString().left(19), "yyyy-MM-ddThh:mm:ss"));
        sr.setSolarMidnight(QPair<QDateTime, double>(QDateTime::fromString(array.at(i).toObject()["solarmidnight"].toObject()["time"].toString().left(19), "yyyy-MM-ddThh:mm:ss"),
                                                     array.at(i).toObject()["solarmidnight"].toObject()["elevation"].toString().toDouble()));
        sr.setSolarNoon(QPair<QDateTime, double>(QDateTime::fromString(array.at(i).toObject()["solarnoon"].toObject()["time"].toString().left(19), "yyyy-MM-ddThh:mm:ss"),
                                                 array.at(i).toObject()["solarnoon"].toObject()["elevation"].toString().toDouble()));
        sr.setHighMoon(QPair<QDateTime, double>(QDateTime::fromString(array.at(i).toObject()["high_moon"].toObject()["time"].toString().left(19), "yyyy-MM-ddThh:mm:ss"),
                                                array.at(i).toObject()["high_moon"].toObject()["elevation"].toString().toDouble()));
        sr.setLowMoon(QPair<QDateTime, double>(QDateTime::fromString(array.at(i).toObject()["low_moon"].toObject()["time"].toString().left(19), "yyyy-MM-ddThh:mm:ss"),
                                               array.at(i).toObject()["low_moon"].toObject()["elevation"].toString().toDouble()));
        sr.setMoonPhase(array.at(i).toObject()["moonposition"].toObject()["phase"].toString().toDouble());
        
        // use proper timezone (not local time)
        sr.setSunSet(QDateTime(sr.sunSet().date(), sr.sunSet().time(), tz));
        sr.setSunRise(QDateTime(sr.sunRise().date(), sr.sunRise().time(), tz));
        sr.setMoonSet(QDateTime(sr.moonSet().date(), sr.moonSet().time(), tz));
        sr.setMoonRise(QDateTime(sr.moonRise().date(), sr.moonRise().time(), tz));
        
        sunrise_.push_back(sr);
    }

    noData = false;
    emit finished();
    reply->deleteLater();
}

void NMISunriseAPI::popDay()
{
    QDateTime today = QDateTime::currentDateTime();
    int i = 0;
    for (const auto &day : qAsConst(sunrise_)) {
        if (day.sunRise().daysTo(today) > 0) {
            i++;
        } else {
            break;
        }
    }
    while(i--)
        sunrise_.pop_front();
};
void NMISunriseAPI::setData(QList<AbstractSunrise> sunrise)
{
    sunrise_ = std::move(sunrise);
    popDay();
    update();
}
bool NMISunriseAPI::isDayTime(QDateTime date)
{
    return NMISunriseAPI::isDayTime(date, this->sunrise_);
}
bool NMISunriseAPI::isDayTime(QDateTime date, QList<AbstractSunrise> sunrise)
{
    for (auto sr : sunrise) {
        // if on the same day
        if (sr.sunRise().date().daysTo(date.date()) == 0 && sr.sunRise().date().day() == date.date().day()) {
            // 30 min threshold
            return sr.sunRise().addSecs(-1800) <= date && sr.sunSet().addSecs(1800) >= date;
        }
    }

    // not found
    return date.time().hour() >= 6 && date.time().hour() <= 18;
}
