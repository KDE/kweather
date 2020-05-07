
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

void LocationQueryModel::textChanged(QString query, int i)
{
    text_ = query;

    emit layoutAboutToBeChanged();
    // clear results list
    for (auto query : resultsList) // memory leak precaution
        delete query;
    resultsList.clear();

    emit layoutChanged();
    if (query != "") { // do not query nothing
        loading_ = true;
        networkError_ = false;
        emit propertyChanged();

        inputTimer->start(i); // make request once input stopped for 2 secs
    }
}

void LocationQueryModel::setQuery()
{
    QUrl url("http://api.geonames.org/searchJSON");
    QUrlQuery urlQuery;

    urlQuery.addQueryItem("q", text_);
    urlQuery.addQueryItem("maxRows", "50");
    urlQuery.addQueryItem("username", "kweatherdev");
    url.setQuery(urlQuery);
    qDebug() << url.toString();
    QNetworkReply *rep = networkAccessManager->get(QNetworkRequest(url));
    connect(rep, &QNetworkReply::finished, this, [this, rep]() { handleQueryResults(rep); });
}

void LocationQueryModel::addLocation(int index)
{
    if (resultsList.front()->latitude() == 1000)
        return; // no result, don't add location
    index_ = index;
    emit appendLocation();
}

void LocationQueryModel::handleQueryResults(QNetworkReply *reply)
{
    loading_ = false;
    if (!reply || reply->error()) {
        networkError_ = true;
        qDebug() << "Network error:" << reply->error();
        emit propertyChanged();
        return;
    }

    loading_ = false;
    networkError_ = false;
    emit propertyChanged();

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject root = document.object();
    // if no result
    if (root[QLatin1String("totalResultsCount")].toInt() == 0) {
        resultsList.append(new LocationQueryResult(1000, 0, QLatin1String("No"), QLatin1String("Result"), QLatin1String("Found"), QString::fromUtf8("¯\(ツ)_/¯"), "ID"));
        return;
    }
    // if our api calls reached daily limit
    if (root[QLatin1String("status")].toObject()[QLatin1String("value")].toInt() == 18) {
        qWarning() << "api calls reached daily limit";
        networkError_ = true;
        emit propertyChanged();
        return;
    }
    emit layoutAboutToBeChanged();
    QJsonArray geonames = root.value("geonames").toArray();
    // add query results
    for (QJsonValueRef resRef : geonames) {
        QJsonObject res = resRef.toObject();
        auto *result = new LocationQueryResult(
            res.value("lat").toString().toFloat(),
            res.value("lng").toString().toFloat(),
            res.value("toponymName").toString(),
            res.value("name").toString(),
            res.value("countryCode").toString(),
            res.value("countryName").toString(),
            QString(res.value("geonameId").toInt()));
        resultsList.append(result);
    }

    emit layoutChanged();
}

void LocationQueryModel::updateUi()
{
}
