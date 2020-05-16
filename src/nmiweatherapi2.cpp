/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "nmiweatherapi2.h"
#include "abstractdailyweatherforecast.h"
#include "abstracthourlyweatherforecast.h"
#include "abstractweatherforecast.h"
#include "geotimezone.h"
#include "sunriseset.h"

#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimeZone>
#include <QUrlQuery>
#include <QXmlStreamReader>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <algorithm>

void NMIWeatherAPI2::setLocation(float latitude, float longitude)
{
    lat = latitude;
    lon = longitude;
    if (timeZone.isEmpty()) {
        tz = new GeoTimeZone(lat, lon);
        connect(tz, &GeoTimeZone::finished, this, &NMIWeatherAPI2::setTZ);
    }
}

void NMIWeatherAPI2::setToken(QString &)
{
} // no token is needed

NMIWeatherAPI2::NMIWeatherAPI2(QString locationId)
    : AbstractWeatherAPI(locationId, -1)
{
    currentData_ = new AbstractWeatherForecast(QDateTime::currentDateTime(), locationId, lat, lon, QList<AbstractHourlyWeatherForecast *>(), QList<AbstractDailyWeatherForecast *>());
}

NMIWeatherAPI2::~NMIWeatherAPI2()
{
    if (!tz)
        delete tz;
}

void NMIWeatherAPI2::update()
{
    if (currentData_ != nullptr && !currentData_->dailyForecasts().empty() && !currentData_->hourlyForecasts().empty() &&
        currentData_->timeCreated().secsTo(QDateTime::currentDateTime()) < 300) { // don't update if updated recently, and forecast is not empty
        emit updated(currentData_);
        return;
    }
    if (rs == nullptr && !timeZone.isEmpty()) {
        qDebug() << "construct sunrise class";
        rs = new SunRiseSet(lat, lon, QTimeZone(QByteArray::fromStdString(timeZone.toStdString())).offsetFromUtc(QDateTime::currentDateTime()));
        connect(rs, &SunRiseSet::finished, this, [this] {
            this->isSunRiseSet = true;
            currentData_->setSunrise(rs->get());
        });
    }
    if (rs) {
        rs->popDay(); // remove old data
        rs->update();
    }
    day_ = 0;
    QUrl url("https://api.met.no/weatherapi/locationforecast/2.0/");
    QUrlQuery query;
    query.addQueryItem("lat", QString::number(lat));
    query.addQueryItem("lon", QString::number(lon));

    url.setQuery(query);

    qDebug() << url;
    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    // see §Identification on https://api.met.no/conditions_service.html
    req.setHeader(QNetworkRequest::UserAgentHeader, QString(QCoreApplication::applicationName() + QLatin1Char(' ') + QCoreApplication::applicationVersion() + QLatin1String(" (kde-pim@kde.org)")));

    // TODO see §Cache on https://api.met.no/conditions_service.html
    // see §Compression on https://api.met.no/conditions_service.html
    //    req.setRawHeader("Accept-Encoding", "gzip, deflate");
    mReply = mManager->get(req);
    connect(
        mReply, &QNetworkReply::finished, this, [this]() { this->parse(this->mReply); }, Qt::ConnectionType::UniqueConnection);
    if (!rs && !timeZone.isEmpty()) {
        rs = new SunRiseSet(lat, lon, QTimeZone(QByteArray::fromStdString(timeZone.toStdString())).offsetFromUtc(QDateTime::currentDateTime()));
        connect(rs, &SunRiseSet::finished, this, [this] { this->isSunRiseSet = true; });
    }
}

void NMIWeatherAPI2::parse(QNetworkReply *reply)
{
    qDebug() << "data arrived";

    // parse json
    QJsonDocument jsonDocument = QJsonDocument::fromJson(reply->readAll());
    reply->deleteLater();

    if (jsonDocument.isObject()) {
        QJsonObject obj = jsonDocument.object();
        QJsonObject prop = obj["properties"].toObject();

        if (prop.contains("timeseries")) {
            QJsonArray timeseries = prop["timeseries"].toArray();

            QHash<QDate, AbstractDailyWeatherForecast *> dayCache;
            QList<AbstractHourlyWeatherForecast *> hoursList;

            // loop over all forecast data
            for (QJsonValueRef ref : timeseries) {
                QJsonObject refObj = ref.toObject();
                parseOneElement(refObj, dayCache, hoursList);
            }

            // delete old data
            delete currentData_;
            // process and build abstract forecast
            currentData_ = new AbstractWeatherForecast(QDateTime::currentDateTime(), locationId_, lat, lon, hoursList, dayCache.values());

            // TODO set location currentData_.setLocation
        }
    }

    // parsing failed, default forecast
    if (currentData_ == nullptr) {
        currentData_ = new AbstractWeatherForecast(QDateTime::currentDateTime(), locationId_, lat, lon, QList<AbstractHourlyWeatherForecast *>(), QList<AbstractDailyWeatherForecast *>());
    }
    // sort daily forecast
    currentData_->sortDailyForecast();
    if (!isTimeZoneSet) {
        if (!timeZone.isEmpty()) {
            for (auto fc : currentData_->hourlyForecasts()) {
                fc->setDate(fc->date().toTimeZone(QTimeZone(QByteArray::fromStdString(timeZone.toStdString()))));
            }
        } else
            emit noTimeZone();
    }
    emit updated(this->currentData());
}

// https://api.met.no/weatherapi/locationforecast/2.0/documentation
void NMIWeatherAPI2::parseOneElement(QJsonObject &object, QHash<QDate, AbstractDailyWeatherForecast *> &dayCache, QList<AbstractHourlyWeatherForecast *> &hoursList)
{
    QJsonObject data = object["data"].toObject(), instant = data["instant"].toObject()["details"].toObject();
    // ignore last forecast, which does not have enough data
    if (!data.contains("next_6_hours") && !data.contains("next_1_hours"))
        return;

    // correct date to corresponding timezone of location if possible
    QDateTime date;
    if (timeZone.isEmpty()) {
        date = QDateTime::fromString(object.value("time").toString(), Qt::ISODate);
        isTimeZoneSet = false;
    } else {
        date = QDateTime::fromString(object.value("time").toString(), Qt::ISODate).toTimeZone(QTimeZone(QByteArray::fromStdString(timeZone.toStdString())));
    }

    auto *hourForecast = new AbstractHourlyWeatherForecast();

    // set initial hour fields
    hourForecast->setDate(date); // the first time will be at the exact time of query, otherwise the beginning of each hour
    hourForecast->setTemperature(instant["air_temperature"].toDouble());
    hourForecast->setPressure(instant["air_pressure_at_sea_level"].toDouble());
    hourForecast->setWindSpeed(instant["wind_speed"].toDouble());
    hourForecast->setHumidity(instant["relative_humidity"].toDouble());
    hourForecast->setFog(instant["fog_area_fraction"].toDouble());
    hourForecast->setUvIndex(instant["ultraviolet_index_clear_sky"].toDouble());

    // wind direction
    double windDirectionDeg = instant["wind_from_direction"].toDouble();
    if (windDirectionDeg < 22.5 || windDirectionDeg >= 337.5) {
        hourForecast->setWindDirection(AbstractHourlyWeatherForecast::S); // from N
    } else if (windDirectionDeg > 22.5 || windDirectionDeg <= 67.5) {
        hourForecast->setWindDirection(AbstractHourlyWeatherForecast::SW); // from NE
    } else if (windDirectionDeg > 67.5 || windDirectionDeg <= 112.5) {
        hourForecast->setWindDirection(AbstractHourlyWeatherForecast::W); // from E
    } else if (windDirectionDeg > 112.5 || windDirectionDeg <= 157.5) {
        hourForecast->setWindDirection(AbstractHourlyWeatherForecast::NW); // from SE
    } else if (windDirectionDeg > 157.5 || windDirectionDeg <= 202.5) {
        hourForecast->setWindDirection(AbstractHourlyWeatherForecast::N); // from S
    } else if (windDirectionDeg > 202.5 || windDirectionDeg <= 247.5) {
        hourForecast->setWindDirection(AbstractHourlyWeatherForecast::NE); // from SW
    } else if (windDirectionDeg > 247.5 || windDirectionDeg <= 292.5) {
        hourForecast->setWindDirection(AbstractHourlyWeatherForecast::E); // from W
    } else if (windDirectionDeg > 292.5 || windDirectionDeg <= 337.5) {
        hourForecast->setWindDirection(AbstractHourlyWeatherForecast::SE); // from NW
    }

    QString symbolCode;
    // some fields contain only "next_1_hours", and others may contain only "next_6_hours"
    if (data.contains("next_1_hours")) {
        QJsonObject nextOneHours = data["next_1_hours"].toObject();
        symbolCode = nextOneHours["summary"].toObject()["symbol_code"].toString("unknown");
        hourForecast->setPrecipitationAmount(nextOneHours["details"].toObject()["precipitation_amount"].toDouble());
    } else {
        QJsonObject nextSixHours = data["next_6_hours"].toObject();
        symbolCode = nextSixHours["summary"].toObject()["symbol_code"].toString("unknown");
        hourForecast->setPrecipitationAmount(nextSixHours["details"].toObject()["precipitation_amount"].toDouble());
    }

    symbolCode = symbolCode.split('_')[0]; // trim _[day/night] from end - https://api.met.no/weatherapi/weathericon/2.0/legends
    hourForecast->setNeutralWeatherIcon(apiDescMap[symbolCode + "_neutral"].icon);

    /* ~~~~~ WIP ~~~~~ */
    // figure out whether to use night or day weather icon and description
    if (isSunRiseSet) {
        qDebug() << "sunrise set";
        if (rs->isDayTime(hourForecast->date())) {
            hourForecast->setWeatherDescription(apiDescMap[symbolCode + "_day"].desc);
            hourForecast->setWeatherIcon(apiDescMap[symbolCode + "_day"].icon);
        } else {
            hourForecast->setWeatherDescription(apiDescMap[symbolCode + "_night"].desc);
            hourForecast->setWeatherIcon(apiDescMap[symbolCode + "_night"].icon);
        }
    } else {
        if (hourForecast->date().time().hour() >= 19 || hourForecast->date().time().hour() <= 6) { // TODO use system sunrise and sunset instead
            hourForecast->setWeatherDescription(apiDescMap[symbolCode + "_night"].desc);
            hourForecast->setWeatherIcon(apiDescMap[symbolCode + "_night"].icon);
        } else {
            hourForecast->setWeatherDescription(apiDescMap[symbolCode + "_day"].desc);
            hourForecast->setWeatherIcon(apiDescMap[symbolCode + "_day"].icon);
        }
    }
    // add day if not already created
    if (!dayCache.contains(date.date())) {
        dayCache[date.date()] = new AbstractDailyWeatherForecast(-1e9, 1e9, 0, 0, 0, 0, "weather-none-available", "", date.date());
    }

    // update day forecast with hour information if needed
    AbstractDailyWeatherForecast *dayForecast = dayCache[date.date()];

    dayForecast->setPrecipitation(dayForecast->precipitation() + hourForecast->precipitationAmount());
    dayForecast->setUvIndex(std::max(dayForecast->uvIndex(), hourForecast->uvIndex()));
    dayForecast->setHumidity(std::max(dayForecast->humidity(), hourForecast->humidity()));
    dayForecast->setPressure(std::max(dayForecast->pressure(), hourForecast->pressure()));

    if (data.contains("next_6_hours")) {
        QJsonObject details = data["next_6_hours"].toObject()["details"].toObject();
        dayForecast->setMaxTemp(std::max(dayForecast->maxTemp(), (float)details["air_temperature_max"].toDouble()));
        dayForecast->setMinTemp(std::min(dayForecast->minTemp(), (float)details["air_temperature_min"].toDouble()));
    }

    // rank weather (for what best describes the day overall)
    QHash<QString, int> rank = {// only need neutral icons
                                {"weather-none-available", -1},
                                {"weather-clear", 0},
                                {"weather-few-clouds", 1},
                                {"weather-clouds", 2},
                                {"weather-fog", 3},
                                {"weather-mist", 3},
                                {"weather-showers-scattered", 4},
                                {"weather-snow-scattered", 4},
                                {"weather-showers", 5},
                                {"weather-hail", 5},
                                {"weather-snow", 5},
                                {"weather-freezing-rain", 6},
                                {"weather-freezing-storm", 6},
                                {"weather-snow-rain", 6},
                                {"weather-storm", 7}};

    // set description and icon if it is higher ranked
    if (rank[hourForecast->neutralWeatherIcon()] >= rank[dayForecast->weatherIcon()]) {
        dayForecast->setWeatherDescription(hourForecast->weatherDescription());
        dayForecast->setWeatherIcon(hourForecast->neutralWeatherIcon());
    }

    // add hour forecast to list
    hoursList.append(hourForecast);
}

void NMIWeatherAPI2::setTZ()
{
    timeZone = tz->getTimeZone();
    qDebug() << "timezone" << timeZone;
    emit timeZoneSet();
}
