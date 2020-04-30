#ifndef WEATHERLOCATIONMODEL_H
#define WEATHERLOCATIONMODEL_H
class WeatherDayListModel;
class WeatherHourListModel;
class AbstractWeatherAPI;

#include <QAbstractListModel>
#include <QObject>

#include "abstractweatherapi.h"
#include "abstractweatherforecast.h"
#include "weatherdaymodel.h"
#include "weatherhourmodel.h"

class WeatherLocation : public QObject
{
    Q_OBJECT
public:
    explicit WeatherLocation();
    explicit WeatherLocation(AbstractWeatherAPI* weatherBackendProvider, QString locationName, float latitude,
                             float longitude);
    explicit WeatherLocation(AbstractWeatherAPI* weatherBackendProvider, QString locationName, float latitude,
                             float longitude, QList<AbstractWeatherForecast*> forecasts_);

    inline QString locationName() { return locationName_; }
    inline float latitude() { return latitude_; }
    inline float longitude() { return longitude_; }

    inline WeatherDayListModel* weatherDayListModel() { return weatherDayListModel_; }
    inline WeatherHourListModel* weatherHourListModel() { return weatherHourListModel_; }
    inline QList<AbstractWeatherForecast*> forecasts() { return forecasts_; }

    inline AbstractWeatherAPI* weatherBackendProvider() { return weatherBackendProvider_; }

public slots:
    void updateData(QList<AbstractWeatherForecast*> fc);

signals:
    void weatherRefresh(QList<AbstractWeatherForecast*> fc); // sent when weather data is refreshed

private:
    QString locationName_;
    float latitude_, longitude_;

    WeatherDayListModel* weatherDayListModel_;
    WeatherHourListModel* weatherHourListModel_;

    AbstractWeatherAPI* weatherBackendProvider_;

    QList<AbstractWeatherForecast*> forecasts_;
};

class WeatherLocationListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit WeatherLocationListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    Q_INVOKABLE void updateUi();

    Q_INVOKABLE void insert(int index, WeatherLocation* weatherLocation);
    Q_INVOKABLE void remove(int index);
    Q_INVOKABLE WeatherLocation* get(int index);
    inline QList<WeatherLocation*>& getList() { return locationsList; };

private:
    QList<WeatherLocation*> locationsList;
};

#endif // WEATHERLOCATIONMODEL_H
