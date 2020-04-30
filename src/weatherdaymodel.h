#ifndef WEATHERDAYMODEL_H
#define WEATHERDAYMODEL_H
class WeatherLocation;

#include <QAbstractListModel>
#include <QObject>

#include "abstractweatherforecast.h"
#include "weatherlocationmodel.h"

// generated from the forecasts list
class WeatherDay : public AbstractWeatherForecast
{
    Q_OBJECT

public:
    explicit WeatherDay();
};

// caches WeatherDays, until signal is called to update and regenerate
class WeatherDayListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit WeatherDayListModel(WeatherLocation* location = nullptr);

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    void refreshDaysFromForecasts();
    Q_INVOKABLE void updateUi();

private:
    QList<WeatherDay*> daysList;
};

#endif // WEATHERDAYMODEL_H
