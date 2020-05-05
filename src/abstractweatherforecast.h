#ifndef ABSTRACTWEATHERFORECAST_H
#define ABSTRACTWEATHERFORECAST_H

#include <QDateTime>
#include <QObject>

class AbstractWeatherForecast : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString locationName READ locationName WRITE setLocationName NOTIFY propertyChanged)
    Q_PROPERTY(QString windDirection READ windDirection WRITE setWindDirection NOTIFY propertyChanged)
    Q_PROPERTY(QString weatherDescription READ weatherDescription WRITE setWeatherDescription NOTIFY propertyChanged)
    Q_PROPERTY(QString weatherIcon READ weatherIcon WRITE setWeatherIcon NOTIFY propertyChanged)
    Q_PROPERTY(QString neutralWeatherIcon READ neutralWeatherIcon WRITE setNeutralWeatherIcon NOTIFY propertyChanged)
    Q_PROPERTY(QDateTime time READ time WRITE setTime NOTIFY propertyChanged)
    Q_PROPERTY(float latitude READ latitude WRITE setLatitude NOTIFY propertyChanged)
    Q_PROPERTY(float longitude READ longitude WRITE setLongitude NOTIFY propertyChanged)
    Q_PROPERTY(float precipitation READ precipitation WRITE setPrecipitation NOTIFY propertyChanged)
    Q_PROPERTY(float fog READ fog WRITE setFog NOTIFY propertyChanged)
    Q_PROPERTY(int cloudiness READ cloudiness WRITE setCloudiness NOTIFY propertyChanged)
    Q_PROPERTY(int windSpeed READ windSpeed WRITE setWindSpeed NOTIFY propertyChanged)
    Q_PROPERTY(int maxTemp READ maxTemp WRITE setMaxTemp NOTIFY propertyChanged)
    Q_PROPERTY(int minTemp READ minTemp WRITE setMinTemp NOTIFY propertyChanged)
    Q_PROPERTY(int humidity READ humidity WRITE setHumidity NOTIFY propertyChanged)
    Q_PROPERTY(int pressure READ pressure WRITE setPressure NOTIFY propertyChanged)

public:
    AbstractWeatherForecast();
    AbstractWeatherForecast(QString locationName, QString windDirection, QString weatherDescription,
                            QString weatherIcon, QString neutralWeatherIcon, QDateTime time, float latitude, float longitude, float precipitation, float fog,
                            int cloudiness, int windSpeed, int maxTemp, int minTemp, int humidity, int pressure);
    const QString& locationName();
    const QString& windDirection();
    const QString& weatherDescription();
    const QString& weatherIcon();
    const QString& neutralWeatherIcon();
    inline const QDateTime time() { return time_; };
    inline float latitude() { return latitude_; };
    inline float longitude() { return longitude_; };
    inline float precipitation() { return precipitation_; };
    inline float fog() { return fog_; };
    inline int cloudiness() { return cloudiness_; };
    inline int windSpeed() { return windSpeed_; };
    inline int maxTemp() { return maxTemp_; };
    inline int minTemp() { return minTemp_; };
    inline int humidity() { return humidity_; };
    inline int pressure() { return pressure_; };
    inline void setLocationName(QString n) { locationName_ = n; };
    inline void setWindDirection(QString d) { windDirection_ = d; };
    inline void setWeatherDescription(QString d) { weatherDescription_ = d; };
    inline void setWeatherIcon(QString i) { weatherIcon_ = i; };
    inline void setNeutralWeatherIcon(QString i) { neutralWeatherIcon_ = i; };
    inline void setTime(QDateTime t) { time_ = t; };
    inline void setLatitude(float l) { latitude_ = l; };
    inline void setLongitude(float l) { longitude_ = l; };
    inline void setPrecipitation(float p) { precipitation_ = p; };
    inline void setFog(float f) { fog_ = f; };
    inline void setCloudiness(int c) { cloudiness_ = c; };
    inline void setWindSpeed(int w) { windSpeed_ = w; };
    inline void setMaxTemp(int m) { maxTemp_ = m; };
    inline void setMinTemp(int m) { minTemp_ = m; };
    inline void setHumidity(int h) { humidity_ = h; };
    inline void setPressure(int p) { pressure_ = p; };

signals:
    void propertyChanged();

private:
    QString locationName_;
    QString windDirection_;
    QString weatherDescription_;
    QString weatherIcon_;
    QString neutralWeatherIcon_; // weather icon without time of day
    QDateTime time_;
    float latitude_;
    float longitude_;
    float precipitation_;
    float fog_;
    int cloudiness_;
    int windSpeed_;
    int maxTemp_;
    int minTemp_;
    int humidity_;
    int pressure_;
};

#endif // ABSTRACTWEATHERFORECAST_H
