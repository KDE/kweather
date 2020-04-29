#ifndef ABSTRACTAPI_H
#define ABSTRACTAPI_H
#include <memory>
#include <QObject>
#include <vector>
#include "abstractweatherforecast.h"
class QNetworkAccessManager;
class QNetworkReply;
class AbstractAPI : public QObject
{
    Q_OBJECT
public:
    AbstractAPI(int interval,QString* token=nullptr, QObject* parent = nullptr);
    virtual ~AbstractAPI();
    virtual void setLocation(float lat, float lon)=0;
    virtual void update()=0;
    const int interval=-1; // api update interval in hour
    virtual void setToken(QString& token)=0;
protected:
    float lat;
    float lon;
    QString* token_=nullptr;
    std::vector<AbstractWeatherForecast*> mForecasts;
    QNetworkAccessManager* mManager;
    QNetworkReply* mReply;
signals:
    void updated();
public slots:
    virtual void parse(QNetworkReply* Reply)=0;
};

#endif // ABSTRACTAPI_H
