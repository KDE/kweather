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
#include <QLocale>
#include <QXmlStreamReader>
KWeather_1x4::KWeather_1x4(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
{
    auto system = QLocale::system().measurementSystem();
    if (system == QLocale::MetricSystem || system == QLocale::ImperialUKSystem)
        m_isCelsius = true;
    else
        m_isCelsius = false;
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
    if (locationList.size() != 0) {
        if (locationList.size() > 1)
            m_isSingleLocation = false;
        else
            m_isSingleLocation = true;
        m_interface = new QDBusInterface("org.kde.kweather", "/locations/" + locationList.first(), "org.kde.kweather.WeatherLocation", QDBusConnection::sessionBus(), this);
        QDBusConnection::sessionBus().connect("org.kde.kweather", "/locations/" + locationList.first(), "org.kde.kweather.WeatherLocation", "currentForecastChange", this, SLOT(update()));

        QDBusReply<QString> reply = m_interface->call("getWeatherData");
        this->parse(QJsonDocument::fromJson(reply.value().toUtf8()));
        this->m_cityName = m_interface->property("name").toString();
        Q_EMIT locationChanged();
    }
    auto today = QDate::currentDate();
    m_currentDate = QDate::currentDate();
    for (int i = 0; i < 4; i++) {
        m_date.append(QLocale::system().standaloneDayName(today.dayOfWeek(), QLocale::ShortFormat));
        today = today.addDays(1);
    }
}

void KWeather_1x4::addCity(QString id)
{
    locationList.append(id);
    m_isSingleLocation = true;
    Q_EMIT locationChanged();
}

void KWeather_1x4::removeCity(QString id)
{
    locationList.removeOne(id);
    if (locationList.size() > 1)
        m_isSingleLocation = false;
    else
        m_isSingleLocation = true;
    Q_EMIT locationChanged();

    m_currentLocationIndex = -1; // so next call will fetch location 0
    this->nextLocation();
}

void KWeather_1x4::parse(QJsonDocument doc)
{
    m_forecast.clear();
    m_maxMinTemp.clear();
    QJsonArray array = doc["dailyForecasts"].toArray();
    QJsonArray hourly = doc["hourlyForecasts"].toArray();
    m_description = hourly.at(0).toObject()["weatherDescription"].toString();
    if (m_isCelsius)
        m_tempNow = QString::number(hourly.at(0).toObject()["temperature"].toDouble()) + "°C";
    else
        m_tempNow = QString::number(hourly.at(0).toObject()["temperature"].toDouble() * 1.8 + 32) + "°";
    int i = 0;
    for (auto days : array) {
        if (i++ >= 4)
            break;
        m_forecast.append(days.toObject()["weatherIcon"].toString());
        if (m_isCelsius)
            m_maxMinTemp.append(QString::number(days.toObject()["maxTemp"].toInt()) + "°C/" + QString::number(days.toObject()["minTemp"].toInt()) + "°C");
        else
            m_maxMinTemp.append(QString::number(days.toObject()["maxTemp"].toInt() * 1.8 + 32) + "°/" + QString::number(days.toObject()["minTemp"].toInt() * 1.8 + 32) + "°");
    }
    Q_EMIT dataUpdated();
    if (m_currentDate.daysTo(QDate::currentDate()) >= 1) {
        auto today = QDate::currentDate();
        m_currentDate = QDate::currentDate();
        for (int i = 0; i < 4; i++) {
            m_date.append(QLocale::system().standaloneDayName(today.dayOfWeek(), QLocale::ShortFormat));
            today = today.addDays(1);
        }
        Q_EMIT dateChanged();
    }
}

void KWeather_1x4::update()
{
    QDBusReply<QString> reply = m_interface->call("getWeatherData");
    this->parse(QJsonDocument::fromJson(reply.value().toUtf8()));
}

void KWeather_1x4::nextLocation()
{
    m_currentLocationIndex++;
    if (m_currentLocationIndex >= locationList.count())
        m_currentLocationIndex = 0;

    auto oldInterface = m_interface;

    m_interface = new QDBusInterface("org.kde.kweather", "/locations/" + locationList.at(m_currentLocationIndex), "org.kde.kweather.WeatherLocation", QDBusConnection::sessionBus(), this);
    QDBusConnection::sessionBus().connect("org.kde.kweather", "/locations/" + locationList.at(m_currentLocationIndex), "org.kde.kweather.WeatherLocation", "currentForecastChange", this, SLOT(update()));

    QDBusReply<QString> reply = m_interface->call("getWeatherData");
    this->parse(QJsonDocument::fromJson(reply.value().toUtf8()));
    this->m_cityName = m_interface->property("name").toString();
    Q_EMIT locationChanged();

    delete oldInterface;
}
K_EXPORT_PLASMA_APPLET_WITH_JSON(kweather_1x4, KWeather_1x4, "metadata.json")

#include "kweather_1x4.moc"
