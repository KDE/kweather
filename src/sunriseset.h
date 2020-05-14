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
class AbstractSunrise;
class SunRiseSet : public QObject
{
    Q_OBJECT
public:
    SunRiseSet(float latitude, float longitude, int offset);
    void update(int days = 1);
    void popDay(int days = 1)
    {
        if (days > sunrise_.count() || days < 0)
            return;
        while (days--) {
            sunrise_.pop_front();
        }
    };
    void setOffset(int offset)
    {
        offset_ = offset;
    }
    QList<AbstractSunrise *> get();
    ~SunRiseSet();

signals:
    void finished();
private slots:
    void process(QNetworkReply *reply);

private:
    float longitude_, latitude_, offset_;
    QNetworkAccessManager *manager;
    QList<AbstractSunrise *> sunrise_;
};

#endif // SUNRISESET_H
