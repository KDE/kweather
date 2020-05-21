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
class NMISunriseAPI : public QObject
{
    Q_OBJECT
public:
    NMISunriseAPI(float latitude, float longitude, int offset);
    void update();
    void popDay();
    void setOffset(int offset)
    {
        offset_ = offset;
    }
    void setData(QList<AbstractSunrise *> sunrise)
    {
        sunrise_ = sunrise;
    };
    QList<AbstractSunrise *> get()
    {
        return sunrise_;
    };
    bool isEmpty() const
    {
        return noData;
    }
    ~NMISunriseAPI();
    bool isDayTime(QDateTime date);
signals:
    void finished();
private slots:
    void process(QNetworkReply *reply);

private:
    float longitude_, latitude_, offset_;
    QNetworkAccessManager *manager = nullptr;
    QList<AbstractSunrise *> sunrise_;
    bool noData = true;
};

#endif // SUNRISESET_H
