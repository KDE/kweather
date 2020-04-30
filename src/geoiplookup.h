#ifndef GEOIPLOOKUP_H
#define GEOIPLOOKUP_H

#include <QObject>
class QNetworkAccessManager;
class QNetworkReply;

/*
 * geoiplookup use Ubuntu's api
 */

class GeoIPLookup : public QObject
{
    Q_OBJECT

public:
    Q_PROPERTY(QString name READ name NOTIFY finished)
    Q_PROPERTY(qreal latitude READ latitude NOTIFY finished)
    Q_PROPERTY(qreal longitude READ longitude NOTIFY finished)
    GeoIPLookup();
    ~GeoIPLookup();
    QString name();
    float latitude();
    float longitude();

signals:

    void finished();

private slots:

    void process(QNetworkReply* reply);

private:
    QNetworkAccessManager* mManager;
    QNetworkReply* mReply;
    QString locationName;
    float latitude_;
    float longitude_;
};

#endif // GEOIPLOOKUP_H
