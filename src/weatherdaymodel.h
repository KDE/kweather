#ifndef WEATHERDAYMODEL_H
#define WEATHERDAYMODEL_H
class WeatherLocation;

#include <QAbstractListModel>
#include <QObject>

#include "abstractweatherforecast.h"
#include "weatherlocationmodel.h"

// generated from the forecasts list
class WeatherDay : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString weatherDescription READ weatherDescription NOTIFY propertyChanged)
    Q_PROPERTY(QString weatherIcon READ weatherIcon NOTIFY propertyChanged)
    Q_PROPERTY(QDate date READ date NOTIFY propertyChanged)
    Q_PROPERTY(int maxTemp READ maxTemp NOTIFY propertyChanged)
    Q_PROPERTY(int minTemp READ minTemp NOTIFY propertyChanged)

public:
    explicit WeatherDay();
    // list of forecasts on this day
    explicit WeatherDay(AbstractDailyWeatherForecast* dailyForecast);

    inline QString weatherDescription() {return weatherDescription_;}
    inline QString weatherIcon() {return weatherIcon_;}
    inline QDate date() {return date_;}
    inline int maxTemp() {return maxTemp_;}
    inline int minTemp() {return minTemp_;}

signals:
    void propertyChanged();

private:
    QString weatherDescription_;
    QString weatherIcon_;
    QDate date_;
    int maxTemp_;
    int minTemp_;
};

// caches WeatherDays, until signal is called to update and regenerate
class WeatherDayListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit WeatherDayListModel(WeatherLocation* location = nullptr);

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    Q_INVOKABLE WeatherDay* get(int index);

    Q_INVOKABLE void updateUi();

public slots:
    void refreshDaysFromForecasts(AbstractWeatherForecast* forecast);
    
private:
    QList<WeatherDay*> daysList;
};

#endif // WEATHERDAYMODEL_H
