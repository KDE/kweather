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
