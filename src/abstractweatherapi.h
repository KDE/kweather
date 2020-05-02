#ifndef ABSTRACTAPI_H
#define ABSTRACTAPI_H
#include "abstractweatherforecast.h"
#include <QObject>
#include <memory>
#include <vector>
class QNetworkAccessManager;
class QNetworkReply;
class AbstractWeatherAPI : public QObject
{
    Q_OBJECT

public:
    explicit AbstractWeatherAPI()
    {
    }
    AbstractWeatherAPI(int interval, QString *token = nullptr, QObject *parent = nullptr);
    virtual ~AbstractWeatherAPI();
    virtual void setLocation(float lat, float lon) = 0;
    virtual void update() = 0;
    const int interval = -1; // api update interval in hour
    virtual void setToken(QString &token) = 0;

protected:
    float lat;
    float lon;
    QString *token_ = nullptr;
    QList<AbstractWeatherForecast *> mForecasts;
    QNetworkAccessManager *mManager;
    QNetworkReply *mReply;

signals:
    void updated(QList<AbstractWeatherForecast *> forecasts);

public slots:

    virtual void parse(QNetworkReply *Reply) = 0;
};

#endif // ABSTRACTAPI_H
