#ifndef GEOTIMEZONE_H
#define GEOTIMEZONE_H

#include <QObject>
class QNetworkReply;
class QNetworkAccessManager;
class GeoTimeZone : public QObject
{
    Q_OBJECT
public:
    GeoTimeZone(float lat, float lon, QObject *parent = nullptr);
    ~GeoTimeZone();
    QString getTimeZone();
signals:
    void finished();
private slots:
    void downloadFinished(QNetworkReply *reply);
private:
    QString tz;
    QNetworkAccessManager* manager;
};

#endif // GEOTIMEZONE_H
