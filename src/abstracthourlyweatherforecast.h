#ifndef KWEATHER_ABSTRACTHOURLYWEATHERFORECAST_H
#define KWEATHER_ABSTRACTHOURLYWEATHERFORECAST_H

#include <QObject>
#include <QDateTime>

class AbstractHourlyWeatherForecast : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString weatherDescription READ weatherDescription WRITE setWeatherDescription NOTIFY propertyChanged)
    Q_PROPERTY(QString weatherIcon READ weatherIcon WRITE setWeatherIcon NOTIFY propertyChanged)
    Q_PROPERTY(QString neutralWeatherIcon READ neutralWeatherIcon WRITE setNeutralWeatherIcon NOTIFY propertyChanged)
    Q_PROPERTY(float temperature READ temperature WRITE setTemperature NOTIFY propertyChanged)
    Q_PROPERTY(float pressure READ pressure WRITE setPressure NOTIFY propertyChanged)
    Q_PROPERTY(int windDirection READ windDirection WRITE windDirection NOTIFY propertyChanged)
    Q_PROPERTY(float windSpeed READ windSpeed WRITE setWindSpeed NOTIFY propertyChanged)
    Q_PROPERTY(float humidity READ humidity WRITE setHumidity NOTIFY propertyChanged)
    Q_PROPERTY(float fog READ fog WRITE setFog NOTIFY propertyChanged)
    Q_PROPERTY(float uvIndex READ uvIndex WRITE setUvIndex NOTIFY propertyChanged)
    Q_PROPERTY(float precipitationAmount READ precipitationAmount WRITE setPrecipitationAmount NOTIFY propertyChanged)

public:
    enum WindDirection {
        N, NW, W, SW, S, SE, E, NE
    };

    AbstractHourlyWeatherForecast() = default;
    AbstractHourlyWeatherForecast(QDateTime date, QString weatherDescription, QString weatherIcon, QString neutralWeatherIcon, float temperature, float pressure, WindDirection windDirection, float windSpeed,
                                  float humidity, float fog, float uvIndex, float precipitationAmount);

    const QDateTime &date() const
    {
        return date_;
    }
    void setDate(const QDateTime &date)
    {
        date_ = date;
    }
    const QString &weatherDescription() const
    {
        return weatherDescription_;
    }
    void setWeatherDescription(const QString &weatherDescription)
    {
        weatherDescription_ = weatherDescription;
    }
    const QString &weatherIcon() const
    {
        return weatherIcon_;
    }
    void setWeatherIcon(const QString &weatherIcon)
    {
        weatherIcon_ = weatherIcon;
    }
    const QString &neutralWeatherIcon() const
    {
        return neutralWeatherIcon_;
    }
    void setNeutralWeatherIcon(const QString &neutralWeatherIcon)
    {
        neutralWeatherIcon_ = neutralWeatherIcon;
    }
    float temperature() const
    {
        return temperature_;
    }
    void setTemperature(float temperature)
    {
        temperature_ = temperature;
    }
    float pressure() const
    {
        return pressure_;
    }
    void setPressure(float pressure)
    {
        pressure_ = pressure;
    }
    WindDirection windDirection() const
    {
        return windDirection_;
    }
    void setWindDirection(WindDirection windDirection)
    {
        windDirection_ = windDirection;
    }
    float windSpeed() const
    {
        return windSpeed_;
    }
    void setWindSpeed(float windSpeed)
    {
        windSpeed_ = windSpeed;
    }
    float humidity() const
    {
        return humidity_;
    }
    void setHumidity(float humidity)
    {
        humidity_ = humidity;
    }
    float fog() const
    {
        return fog_;
    }
    void setFog(float fog)
    {
        fog_ = fog;
    }
    float uvIndex() const
    {
        return uvIndex_;
    }
    void setUvIndex(float uvIndex)
    {
        uvIndex_ = uvIndex;
    }
    float precipitationAmount() const
    {
        return precipitationAmount_;
    }
    void setPrecipitationAmount(float precipitationAmount)
    {
        precipitationAmount_ = precipitationAmount;
    }

signals:
    void propertyChanged();

private:
    QDateTime date_;
    QString weatherDescription_;
    QString weatherIcon_;
    QString neutralWeatherIcon_; // weather icon without time of day
    float temperature_{}; // celsius
    float pressure_{}; // hPa
    WindDirection windDirection_;
    float windSpeed_{}; // m/s
    float humidity_{}; // %
    float fog_{}; // %
    float uvIndex_{}; // 0-1
    float precipitationAmount_{}; // mm
};

#endif // KWEATHER_ABSTRACTHOURLYWEATHERFORECAST_H
