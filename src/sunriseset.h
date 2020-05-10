#ifndef SUNRISESET_H
#define SUNRISESET_H

#include <QObject>
#include <QDateTime>
class QNetworkAccessManager;
class QNetworkReply;
class SunRiseSet:public QObject
{
    Q_OBJECT
public:
    SunRiseSet(float latitude, float longitude, int offset);
    ~SunRiseSet();
    int sunSet(int day); // day: offset to current day, start from 0
    int sunRise(int day); // return value is hour of sunrise time, eg. 6
signals:
    void finished();
private slots:
    void process(QNetworkReply* reply);
private:
    QNetworkAccessManager* manager;
    QList<int> sunRise_;
    QList<int> sunSet_;
};

#endif // SUNRISESET_H
