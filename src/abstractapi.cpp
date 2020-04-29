#include "abstractapi.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
AbstractAPI::AbstractAPI(int interval, QString *token, QObject *parent) :
  QObject(parent),
  interval(interval),
  token_(token)
{
  mManager = new QNetworkAccessManager();
}

AbstractAPI::~AbstractAPI() {
  delete  mManager;

  // delete all data
  for (auto d : mForecasts) {
    delete d;
  }
}

QList<AbstractWeatherForecast *> AbstractAPI::updated(){
    return mForecasts;
}
