#include "openweathermap.h"
#include <vector>
#include <memory>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QUrlQuery>
OpenWeatherMap::OpenWeatherMap() :
  AbstractAPI(3)
{}

void OpenWeatherMap::setToken(QString& token) {
  token_ = &token;
}

void OpenWeatherMap::parse(QNetworkReply *reply) {
  QJsonDocument mJson  = QJsonDocument::fromJson(reply->readAll());
  QJsonArray    mArray = mJson["list"].toArray();
  QDateTime     start;

  start.setSecsSinceEpoch(mArray.at(0)["dt"].toInt());
  start = start.toOffsetFromUtc(mJson["city"]["timezone"].toInt());

  for (int i = 0; i < 39; i++) {
    mForecasts.push_back(new AbstractWeatherForecast(
                           QString(mJson["city"]["country"].toString() + ", " +
                                   mJson["city"]["name"].toString()),
                           getWindDirection(mArray.at(i)["wind"]["deg"].toDouble()),
                           mArray.at(i)["weather"]["description"].toString(),
                           map[mArray.at(i)["weather"]["icon"].toString()],
                           start.addSecs(3600 * i),
                           lat,
                           lon,
                           mArray.at(i)["rain"]["3h"].toDouble() +
                           mArray.at(i)["snow"]["3h"].toDouble(),
                           -1.0, // api doesn't support fog
                           mArray.at(i)["clouds"]["all"].toInt(),
                           mArray.at(i)["wind"]["speed"].toInt(),
                           mArray.at(i)["main"]["temp_max"].toInt(),
                           mArray.at(i)["main"]["temp_min"].toInt(),
                           mArray.at(i)["main"]["humidity"].toInt(),
                           mArray.at(i)["main"]["grnd_level"].toInt()
                           ));
  }
}

QString OpenWeatherMap::getWindDirection(double degrees) {
  {
    QString direction;

    if ((degrees >= 0) && (degrees <= 30)) direction = "N";
    else if ((degrees > 30) && (degrees <= 60)) direction = "NE";
    else if ((degrees > 60) && (degrees <= 120)) direction = "E";
    else if ((degrees > 120) && (degrees <= 150)) direction = "SE";
    else if ((degrees > 150) && (degrees <= 210)) direction = "S";
    else if ((degrees > 210) && (degrees <= 240)) direction = "SW";
    else if ((degrees > 240) && (degrees <= 300)) direction = "W";
    else if ((degrees > 300) && (degrees <= 330)) direction = "NW";
    else if ((degrees > 330) && (degrees <= 360)) direction = "N";
    return direction;
  }
}

void OpenWeatherMap::update() {
  QUrlQuery query;

  query.addQueryItem(QStringLiteral("lat"),   QString().setNum(lat));
  query.addQueryItem(QStringLiteral("lat"),   QString().setNum(lon));
  query.addQueryItem(QStringLiteral("APPID"), *token_);

  QUrl url;

  url.setScheme(QStringLiteral("http"));
  url.setHost(QStringLiteral("api.openweathermap.org"));
  url.setPath(QStringLiteral("/forecast"));

  QNetworkRequest req(url);
  mReply = mManager->get(req);
  connect(mManager, &QNetworkAccessManager::finished, this,
          &OpenWeatherMap::parse);
}
