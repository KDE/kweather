#ifndef GEOIPLOOKUP_H
#define GEOIPLOOKUP_H

#include <QObject>
class QNetworkAccessManager;
class QNetworkReply;

/*
 * geoiplookup use Ubuntu's api
 */
class geoiplookup : public QObject {
  Q_OBJECT

public:

  Q_PROPERTY(QString name READ name NOTIFY finished)
  Q_PROPERTY(real lat READ lat NOTIFY finished)
  Q_PROPERTY(real lon READ lat NOTIFY finished)
  geoiplookup();
  QString name();
  float   lat();
  float   lon();

signals:

  void finished();

private slots:

  void process(QNetworkReply *reply);

private:

  QNetworkAccessManager *mManager;
  QNetworkReply *mReply;
  QString locationName;
  float Lat;
  float Lon;
};

#endif // GEOIPLOOKUP_H
