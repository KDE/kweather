/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

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
