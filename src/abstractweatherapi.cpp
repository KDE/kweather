#include "abstractweatherapi.h"
#include "abstractdailyweatherforecast.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
AbstractWeatherAPI::AbstractWeatherAPI(int interval, QString *token, QObject *parent)
    : QObject(parent)
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
