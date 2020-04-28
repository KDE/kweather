#include "geolocation.h"
#include "weathertile.h"
#include <QUrlQuery>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
geolocation::geolocation(QObject *parent) : QObject(parent)
{}

void geolocation::setName(QString& location) {
  request  = &location;
  mManager = new QNetworkAccessManager();
  QUrl url;
  url.setScheme(QStringLiteral("https"));
  url.setHost(QStringLiteral("nominatim.openstreetmap.org"));
  url.setPath(QStringLiteral("/search"));
  QUrlQuery query;
  query.addQueryItem(QStringLiteral("q"),      location);
  query.addQueryItem(QStringLiteral("format"), "json");
  url.setQuery(query);
  QNetworkRequest req(url);
  mReply = mManager->get(req);
  connect(mManager, &QNetworkAccessManager::finished, this,
          &geolocation::process);
}

void geolocation::process(QNetworkReply *reply) {
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

QStringList geolocation::getLocation() {
  return mLocation;
}

void geolocation::setLocation(int i) {
  Lat = cityArray.at(i)["boundingbox"]["lat"].toDouble();
  Lon = cityArray.at(i)["boundingbox"]["lon"].toDouble();
}

float geolocation::lat() {
  return Lat;
}

float geolocation::lon() {
  return Lon;
}
