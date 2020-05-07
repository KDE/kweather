#ifndef ABSTRACTAPI_H
#define ABSTRACTAPI_H
#include "abstractweatherforecast.h"
#include <QObject>
#include <memory>
#include <utility>
#include <vector>
class QNetworkAccessManager;
class QNetworkReply;
class AbstractDailyWeatherForecast;
class AbstractWeatherAPI : public QObject
{
    Q_OBJECT

public:
    explicit AbstractWeatherAPI(QString locationId, AbstractWeatherForecast *currentData = nullptr)
    {
        locationId_ = std::move(locationId);
        currentData_ = currentData;
    }
    AbstractWeatherAPI(QString locationId, int interval, QString *token = nullptr, QObject *parent = nullptr);
    virtual ~AbstractWeatherAPI();
    virtual void setLocation(float lat, float lon) = 0;
    virtual void update() = 0;
    virtual QString &getTimeZone() = 0;
    const int interval = -1; // api update interval in hour
    virtual void setToken(QString &token) = 0;
    AbstractWeatherForecast *currentData()
    {
        return currentData_;
    }
    void setCurrentData(AbstractWeatherForecast *forecast)
    {
        currentData_ = forecast;
    }
    QString locationName()
    {
        return locationId_;
    }
    void setLocationId(QString locationId)
    {
        locationId_ = locationId;
    }

protected:
    QString locationId_;
    float lat;
    float lon;
    QString *token_ = nullptr;
    QNetworkAccessManager *mManager;
    QNetworkReply *mReply;

    AbstractWeatherForecast *currentData_;

signals:
    void updated(AbstractWeatherForecast *forecast);

public slots:

    virtual void parse(QNetworkReply *Reply) = 0;
};

#endif // ABSTRACTAPI_H
