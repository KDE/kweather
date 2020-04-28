#ifndef WEATHERHOURMODEL_H
#define WEATHERHOURMODEL_H

#include <QObject>
#include <QAbstractListModel>

#include <abstractweatherforecast.h>
#include <weatherlocationmodel.h>

class WeatherHour : public AbstractWeatherForecast
{
    Q_OBJECT
    
public:
    explicit WeatherHour();
    
};

class WeatherHourListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit WeatherHourListModel(WeatherLocation* location);
    
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    
    void refreshHoursFromForecasts();
    Q_INVOKABLE void updateUi();
    
private:
    QList<WeatherHour*> hoursList;
};

#endif // WEATHERHOURMODEL_H
