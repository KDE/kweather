#include "abstractapi.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
AbstractAPI::AbstractAPI() : interval(1)
{
  mManager = new QNetworkAccessManager();
}

AbstractAPI::~AbstractAPI() {
  delete  mManager;
}
