
#include "locationquerymodel.h"
#include <QTimer>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtNetwork/QNetworkConfigurationManager>
LocationQueryModel::LocationQueryModel()
{
    QNetworkConfigurationManager ncm;
    this->networkAccessManager = new QNetworkAccessManager(this);
    this->networkSession = new QNetworkSession(ncm.defaultConfiguration(), this);
    inputTimer = new QTimer(this);
    inputTimer->setSingleShot(true);
    connect(inputTimer, &QTimer::timeout, this, &LocationQueryModel::setQuery);
}

int LocationQueryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return resultsList.count();
}

QVariant LocationQueryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto query = resultsList[index.row()];

    if (role == NameRole) {
        return query->name() + ", " + query->countryName();
    }

    return QVariant();
}

QHash<int, QByteArray> LocationQueryModel::roleNames() const
{
    return {{NameRole, "name"}};
}

LocationQueryResult *LocationQueryModel::get(int index)
{
    if (index < 0 || index >= resultsList.count())
        return {};
    return resultsList.at(index);
}

void LocationQueryModel::textChanged(QString query)
{
    text_ = query;
    qDebug() << text_;
    inputTimer->start(2000); // make request once input stopped for 2 secs
}

void LocationQueryModel::setQuery()
{
    QUrl url("http://api.geonames.org/searchJSON");
    QUrlQuery urlQuery;

    urlQuery.addQueryItem("q", text_);
    urlQuery.addQueryItem("maxRows", "50");
    urlQuery.addQueryItem("username", "kweatherdev");
    url.setQuery(urlQuery);

    QNetworkReply *rep = networkAccessManager->get(QNetworkRequest(url));
    connect(rep, &QNetworkReply::finished, this, [this, rep]() { handleQueryResults(rep); });
}

void LocationQueryModel::addLocation(int index)
{
    index_ = index;
    emit appendLocation();
}

void LocationQueryModel::handleQueryResults(QNetworkReply *reply)
{
    if (!reply || reply->error()) {
        return;
    }

    emit layoutAboutToBeChanged();
    for (auto query : resultsList) // memory leak precaution
        delete query;
    resultsList.clear();

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject root = document.object();

    QJsonArray geonames = root.value("geonames").toArray();
    // add query results
    for (QJsonValueRef resRef : geonames) {
        QJsonObject res = resRef.toObject();
        auto *result = new LocationQueryResult(
            res.value("latitude").toDouble(), res.value("longitude").toDouble(), res.value("toponymName").toString(), res.value("name").toString(), res.value("countryCode").toString(), res.value("countryName").toString());
        resultsList.append(result);
    }

    emit layoutChanged();
}

void LocationQueryModel::updateUi()
{
}
