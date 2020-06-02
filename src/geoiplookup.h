/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

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
    GeoIPLookup();
    ~GeoIPLookup();
    QString name();
    float latitude();
    float longitude();
    QString timeZone();
signals:
    void networkError();
    void finished();

private slots:

    void process(QNetworkReply *reply);

private:
    QNetworkAccessManager *mManager = nullptr;
    QNetworkReply *mReply = nullptr;
    QString locationName;
    QString timeZone_;
    float latitude_;
    float longitude_;
};

#endif // GEOIPLOOKUP_H
