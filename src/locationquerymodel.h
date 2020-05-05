#ifndef KWEATHER_LOCATIONQUERYMODEL_H
#define KWEATHER_LOCATIONQUERYMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QtCore/QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkSession>
class QTimer;
// fetched from geonames
class LocationQueryResult : public QObject
{
    Q_OBJECT

public:
    explicit LocationQueryResult()
    {
    }
    explicit LocationQueryResult(float latitude, float longitude, QString toponymName, QString name, QString countryCode, QString countryName)
    {
        this->latitude_ = latitude;
        this->longitude_ = longitude;
        this->toponymName_ = toponymName;
        this->name_ = name;
        this->countryCode_ = countryCode;
        this->countryName_ = countryName;
    }

    inline float latitude()
    {
        return latitude_;
    }
    inline float longitude()
    {
        return longitude_;
    }
    inline QString toponymName()
    {
        return toponymName_;
    }
    inline QString name()
    {
        return name_;
    }
    inline QString countryCode()
    {
        return countryCode_;
    }
    inline QString countryName()
    {
        return countryName_;
    }

private:
    float latitude_, longitude_;
    QString toponymName_, name_, countryCode_, countryName_;
};

class LocationQueryModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool loading READ loading NOTIFY propertyChanged)
    Q_PROPERTY(bool networkError READ networkError NOTIFY propertyChanged)

public:
    explicit LocationQueryModel();
    enum Roles {
        NameRole = Qt::DisplayRole,
    };

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE LocationQueryResult *get(int index);
    Q_INVOKABLE bool loading() {return loading_;}
    Q_INVOKABLE bool networkError() {return networkError_;}
    Q_INVOKABLE void textChanged(QString query, int i = 2000);
    void setQuery();
    Q_INVOKABLE void addLocation(int index);
    Q_INVOKABLE void updateUi();
    int index_;
signals:
    void propertyChanged();
    void appendLocation();
public slots:
    void handleQueryResults(QNetworkReply *reply);

private:
    bool loading_ = false, networkError_ = false;

    QList<LocationQueryResult *> resultsList;
    QTimer *inputTimer;
    QString text_;
    QNetworkAccessManager *networkAccessManager;
    QNetworkSession *networkSession;
};

#endif // KWEATHER_LOCATIONQUERYMODEL_H
