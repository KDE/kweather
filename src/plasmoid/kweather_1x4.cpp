/*
    SPDX-FileCopyrightText: 2020 HanY <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#include "kweather_1x4.h"
#include <QDBusInterface>
#include <QDBusReply>
#include <QDate>
#include <QJsonArray>
#include <QJsonDocument>
#include <QXmlStreamReader>
KWeather_1x4::KWeather_1x4(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
{
    QDBusConnection::sessionBus().connect("org.kde.kweather", "/", "org.kde.kweather.LocationModel", "removed", this, SLOT(addCity(QString)));
    QDBusConnection::sessionBus().connect("org.kde.kweather", "/", "org.kde.kweather.LocationModel", "added", this, SLOT(removeCity(QString)));
    QDBusInterface *interface = new QDBusInterface("org.kde.kweather", "/locations", "org.freedesktop.DBus.Introspectable", QDBusConnection::sessionBus(), this);
    QDBusReply<QString> reply = interface->call("Introspect");
    if (reply.isValid()) {
        auto xmlMsg = reply.value();
        QXmlStreamReader xml(xmlMsg);
        while (!xml.atEnd()) {
            xml.readNext();
            if (xml.name() == "node" && xml.attributes().hasAttribute("name"))
                locationList.append(xml.attributes().value("name").toString());
        }
    }
    if (locationList.count() != 0) {
        m_interface = new QDBusInterface("org.kde.kweather", "/locations/" + locationList.first(), "org.kde.kweather.WeatherLocation", QDBusConnection::sessionBus(), this);
        QDBusConnection::sessionBus().connect("org.kde.kweather", "/locations/" + locationList.first(), "org.kde.kweather.WeatherLocation", "currentForecastChange", this, SLOT(update()));

        QDBusReply<QString> reply = m_interface->call("getWeatherData");
        this->parse(QJsonDocument::fromJson(reply.value().toUtf8()));
        this->m_cityName = m_interface->property("name").toString();
        Q_EMIT locationChanged();
    }
    auto today = QDate::currentDate();
    for (int i = 0; i < 4; i++) {
        m_date.append(today.toString("ddd"));
        today = today.addDays(1);
    }
}

void KWeather_1x4::addCity(QString id)
{
    locationList.append(id);
}

void KWeather_1x4::removeCity(QString id)
{
    for (int i = 0; i < locationList.count(); i++) {
        if (locationList.at(i) == id) {
            locationList.removeAt(i);
            Q_EMIT locationChanged();
            break;
        }
    }
}

void KWeather_1x4::parse(QJsonDocument doc)
{
    m_forecast.clear();
    m_maxMinTemp.clear();
    QJsonArray array = doc["dailyForecasts"].toArray();
    QJsonArray hourly = doc["hourlyForecasts"].toArray();
    m_description = hourly.at(0).toObject()["weatherDescription"].toString();
    m_tempNow = QString::number(hourly.at(0).toObject()["temperature"].toDouble()) + "°C";
    int i = 0;
    for (auto days : array) {
        if (i++ >= 4)
            break;
        m_forecast.append(days.toObject()["weatherIcon"].toString());
        m_maxMinTemp.append(QString::number(days.toObject()["maxTemp"].toDouble()) + "°C|" + QString::number(days.toObject()["minTemp"].toDouble()) + "°C");
    }
    Q_EMIT dataUpdated();
}

void KWeather_1x4::update()
{
    QDBusReply<QString> reply = m_interface->call("getWeatherData");
    this->parse(QJsonDocument::fromJson(reply.value().toUtf8()));
}

K_EXPORT_PLASMA_APPLET_WITH_JSON(kweather_1x4, KWeather_1x4, "metadata.json")

#include "kweather_1x4.moc"
