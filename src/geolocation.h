#ifndef GEOLOCATION_H
#define GEOLOCATION_H

#include <QObject>
#include <QJsonArray>

/*####################################################
 * This class is used for get latitude and longitude
 * from a city name
 * After processing the result, finished() signal is
 * emitted
 * getLocation() will return QStringList contains all
 * possible cities user can choose from
 * setLocation takes a int value which is the position
 * of choosen city in QStringList, and set latitude
 * and longitude
 * latitude() and longitude() return them
 * If no result is found, noResult() is emitted
 * ###################################################
 */

class QNetworkReply;
class QNetworkAccessManager;
class GeoLocation : public QObject {
  Q_OBJECT

public:

  Q_PROPERTY(QStringList possibleLocations READ getLocation NOTIFY finished)
  Q_PROPERTY(qreal latitude READ latitude)
  Q_PROPERTY(qreal longitude READ longitude)
  GeoLocation(QObject *parent = nullptr);
  ~GeoLocation();
  QStringList      getLocation();
  Q_INVOKABLE void setLocation(int i);
  float            latitude();
  float            longitude();
  Q_INVOKABLE void setName(QString& location);

signals:

  void finished();
  void noResult();

private slots:

  void process(QNetworkReply *reply);

private:

  float Lat = 0.0;
  float Lon = 0.0;
  QStringList mLocation;
  QNetworkAccessManager *mManager = nullptr;
  QNetworkReply *mReply           = nullptr;
  QJsonArray cityArray;
};

#endif // GEOLOCATION_H
