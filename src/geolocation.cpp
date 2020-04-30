#include "geolocation.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>

GeoLocation::GeoLocation(QObject* parent)
    : QObject(parent)
{
    mManager = new QNetworkAccessManager();
}

void GeoLocation::setName(QString& location)
{
    QUrl url;

    url.setScheme(QStringLiteral("https"));
    url.setHost(QStringLiteral("nominatim.openstreetmap.org"));
    url.setPath(QStringLiteral("/search"));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("q"), location);
    query.addQueryItem(QStringLiteral("format"), "json");
    url.setQuery(query);
    QNetworkRequest req(url);
    mReply = mManager->get(req);
    connect(mManager, &QNetworkAccessManager::finished, this, &GeoLocation::process);
}

void GeoLocation::process(QNetworkReply* reply)
{
    if (mLocation.isEmpty())
        mLocation.clear();
    QJsonDocument data = QJsonDocument::fromJson(reply->readAll());

    if (data.isEmpty()) {
        emit noResult();
    }
    cityArray = data.array();
    QStringList mLocation = QStringList();

    for (int i = 0; i < cityArray.count(); i++) {
        QString j = cityArray.at(i)["boundingbox"]["display_name"].toString();

        if (j == mLocation.last()) {
            continue;
        }
        mLocation.append(cityArray.at(i)["boundingbox"]["display_name"].toString());
    }
    reply->deleteLater();
    emit finished();
}

QStringList GeoLocation::getLocation()
{
    return mLocation;
}

void GeoLocation::setLocation(int i)
{
    Lat = cityArray.at(i)["boundingbox"]["lat"].toDouble();
    Lon = cityArray.at(i)["boundingbox"]["lon"].toDouble();
}

float GeoLocation::latitude()
{
    return Lat;
}

float GeoLocation::longitude()
{
    return Lon;
}

GeoLocation::~GeoLocation()
{
    delete mManager;
}
