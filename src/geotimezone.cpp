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
    QNetworkRequest req(url);
    manager->get(req);
    connect(manager, &QNetworkAccessManager::finished, this, &GeoTimeZone::downloadFinished);
}

void GeoTimeZone::downloadFinished(QNetworkReply *reply)
{
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    tz = doc["timezoneID"].toString();
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
