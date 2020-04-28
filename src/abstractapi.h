#ifndef ABSTRACTAPI_H
#define ABSTRACTAPI_H
#include <memory>
#include <QObject>
#include <vector>
#include "abstractweatherforecast.h"
class QNetworkAccessManager;
class QNetworkReply;
class AbstractAPI
{
    Q_OBJECT
public:
    AbstractAPI();
    virtual ~AbstractAPI();
    virtual void update()=0;
    const int interval; // api update interval in hour
    virtual void setToken(QString& token)=0;
signals:
    void updated();
private slots:
    virtual void parse()=0;
private:
    std::shared_ptr<std::vector<std::shared_ptr<AbstractWeatherforecast>>> mForecasts;
    QNetworkAccessManager* mManager;
    QNetworkReply* mReply;
    QString* token=nullptr;
};

#endif // ABSTRACTAPI_H
