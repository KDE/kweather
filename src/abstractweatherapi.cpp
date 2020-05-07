#include "abstractweatherapi.h"
#include "abstractdailyweatherforecast.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <utility>
AbstractWeatherAPI::AbstractWeatherAPI(QString locationId, int interval, QString *token, QObject *parent)
    : QObject(parent)
    , locationId_(std::move(locationId))
    , interval(interval)
    , token_(token)
{
    mManager = new QNetworkAccessManager();
}

AbstractWeatherAPI::~AbstractWeatherAPI()
{
    delete mManager;
    delete currentData_;
}
