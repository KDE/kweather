/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#ifndef SUNRISESET_H
#define SUNRISESET_H

#include <QDateTime>
#include <QObject>
class QNetworkAccessManager;
class QNetworkReply;
class SunRiseSet : public QObject
{
    Q_OBJECT
public:
    SunRiseSet(float latitude, float longitude, int offset);
    ~SunRiseSet();
    int sunSet();
    int sunRise();
signals:
    void finished();
private slots:
    void process(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    int sunRise_;
    int sunSet_;
};

#endif // SUNRISESET_H
