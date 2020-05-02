#ifndef WEATHERHOURMODEL_H
#define WEATHERHOURMODEL_H
class WeatherLocation;

#include <QAbstractListModel>
#include <QObject>
#include <utility>

#include "abstractweatherforecast.h"
#include "weatherlocationmodel.h"

class WeatherHour : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString windDirection READ windDirection WRITE setWindDirection NOTIFY propertyChanged)
    Q_PROPERTY(QString weatherDescription READ weatherDescription WRITE setWeatherDescription NOTIFY propertyChanged)
    Q_PROPERTY(QString weatherIcon READ weatherIcon WRITE setWeatherIcon NOTIFY propertyChanged)
    Q_PROPERTY(float precipitation READ precipitation WRITE setPrecipitation NOTIFY propertyChanged)
    Q_PROPERTY(float fog READ fog WRITE setFog NOTIFY propertyChanged)
    Q_PROPERTY(int cloudiness READ cloudiness WRITE setCloudiness NOTIFY propertyChanged)
    Q_PROPERTY(int windSpeed READ windSpeed WRITE setWindSpeed NOTIFY propertyChanged)
    Q_PROPERTY(int temperature READ temperature WRITE setTemperature NOTIFY propertyChanged)
    Q_PROPERTY(int humidity READ humidity WRITE setHumidity NOTIFY propertyChanged)
    Q_PROPERTY(int pressure READ pressure WRITE setPressure NOTIFY propertyChanged)
    Q_PROPERTY(int hour READ hour WRITE setHour NOTIFY propertyChanged)
    Q_PROPERTY(int day READ day WRITE setDay NOTIFY propertyChanged)

public:
    explicit WeatherHour();
    explicit WeatherHour(AbstractWeatherForecast* forecast);
    
    inline QString windDirection() {return windDirection_;}
    inline QString weatherDescription() {return weatherDescription_;}
    inline QString weatherIcon() {return weatherIcon_;}
    inline float precipitation() const {return precipitation_;}
    inline float fog() const {return fog_;}
    inline int cloudiness() const {return cloudiness_;}
    inline int windSpeed() const {return windSpeed_;}
    inline int temperature() const {return temperature_;}
    inline int humidity() const {return humidity_;}
    inline int pressure() const {return pressure_;}
    inline int hour() const {return hour_;}
    inline int day() const {return day_;}

    inline void setWindDirection(QString windDirection) { this->windDirection_ = std::move(windDirection); }
    inline void setWeatherDescription(QString weatherDescription) { this->weatherDescription_ = std::move(weatherDescription); }
    inline void setWeatherIcon(QString weatherIcon) { this->weatherIcon_ = std::move(weatherIcon); }
    inline void setPrecipitation(float precipitation) { this->precipitation_ = precipitation; }
    inline void setFog(float fog) {this->fog_ = fog;}
    inline void setCloudiness(int cloudiness) {this->cloudiness_ = cloudiness;}
    inline void setWindSpeed(int windSpeed) {this->windSpeed_ = windSpeed;}
    inline void setTemperature(int temperature) {this->temperature_ = temperature;}
    inline void setHumidity(int humidity) {this->humidity_ = humidity;}
    inline void setPressure(int pressure) {this->pressure_ = pressure;}
    inline void setHour(int hour) {this->hour_ = hour;}
    inline void setDay(int day) {this->day_ = day;}

    static bool compare(const WeatherHour &wh1, const WeatherHour &wh2)
    {
        return wh1.hour() < wh2.hour();
    }

signals:
    void propertyChanged();

private:
    QString windDirection_;
    QString weatherDescription_;
    QString weatherIcon_;
    float precipitation_;
    float fog_;
    int cloudiness_;
    int windSpeed_;
    int temperature_;
    int humidity_;
    int pressure_;
    
    int hour_;
    int day_;
};

class WeatherHourListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit WeatherHourListModel(WeatherLocation* location = nullptr);

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    Q_INVOKABLE WeatherHour* get(int index);

    Q_INVOKABLE void updateUi();
    
public slots:
    void refreshHoursFromForecasts(QList<AbstractWeatherForecast*> forecasts);
    
private:
    QList<WeatherHour*> hoursList;
};

#endif // WEATHERHOURMODEL_H
