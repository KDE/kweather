#ifndef WEATHERLOCATIONMODEL_H
#define WEATHERLOCATIONMODEL_H

#include <QObject>
#include <QAbstractListModel>

#include <abstractweatherforecast.h>
#include <weatherdaymodel.h>
#include <weatherhourmodel.h>

class WeatherLocation : public QObject
{
    Q_OBJECT
    
public:
    explicit WeatherLocation(QString locationName, float latitude, float longitude);
    explicit WeatherLocation(QString locationName, float latitude, float longitude, QList<AbstractWeatherForecast*> forecasts_);
    
    inline QString locationName() { return locationName_; }
    inline float latitude() { return latitude_; }
    inline float longitude() { return longitude_; }
    
    inline WeatherDayListModel* weatherDayListModel() { return weatherDayListModel_; }
    inline WeatherHourListModel* weatherHourListModel() { return weatherHourListModel_; }
    inline QList<AbstractWeatherForecast*> forecasts() { return forecasts_; }
    
signals:
    void weatherRefresh(); // sent when weather data is refreshed
    
private:
    QString locationName_;
    float latitude_, longitude_;
  
    WeatherDayListModel* weatherDayListModel_;
    WeatherHourListModel* weatherHourListModel_;
    
    QList<AbstractWeatherForecast*> forecasts_;
};

class WeatherLocationListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit WeatherLocationListModel(QObject *parent = nullptr);
    
    int rowCount(const QModelIndex &parent) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;
    
    Q_INVOKABLE void updateUi();
    
    Q_INVOKABLE void insert(int index, WeatherLocation* weatherLocation);
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE WeatherLocation* get(int index);
    
private:
    QList<WeatherLocation*> locationsList;
};

#endif // WEATHERLOCATIONMODEL_H
