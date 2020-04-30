#ifndef WEATHERHOURMODEL_H
#define WEATHERHOURMODEL_H
class WeatherLocation;

#include <QAbstractListModel>
#include <QObject>

#include "abstractweatherforecast.h"
#include "weatherlocationmodel.h"

class WeatherHour : public QObject
{
    Q_OBJECT

public:
    explicit WeatherHour();
    explicit WeatherHour(AbstractWeatherForecast* forecast);
    
private:
    QString windDirection_;
    QString weatherDescription_;
    QString weatherIcon_;
    float precipitation_;
    float fog_;
    int cloudiness_;
    int windSpeed_;
    int temperature;
    int humidity_;
    int pressure_;
    
    int hour;
    int day;
};

class WeatherHourListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit WeatherHourListModel(WeatherLocation* location = nullptr);

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    Q_INVOKABLE void updateUi();
    
public slots:
    void refreshHoursFromForecasts(QList<AbstractWeatherForecast*> forecasts);
    
private:
    QList<WeatherHour*> hoursList;
};

#endif // WEATHERHOURMODEL_H
