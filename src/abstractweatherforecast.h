#ifndef ABSTRACTWEATHERFORECAST_H
#define ABSTRACTWEATHERFORECAST_H

#include <QDateTime>
#include <QObject>

class AbstractWeatherForecast : public QObject
{
    Q_OBJECT
public:
    AbstractWeatherForecast();
    AbstractWeatherForecast(QString LocationName, QString WindDirection, QString WeatherDescription,
                            QString WeatherIcon, QDateTime Time, float Lat, float Lon, float Precipitation, float Fog,
                            int Cloudiness, int WindSpeed, int MaxTemp, int MinTemp, int Humidity, int Pressure);
    const QString& locationName();
    const QString& windDirection();
    const QString& weatherDescription();
    const QString& weatherIcon();
    inline const QDateTime time() { return time_; };
    inline float lat() { return lat_; };
    inline float lon() { return lon_; };
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
    inline void setTime(QDateTime t) { time_ = t; };
    inline void setLat(float l) { lat_ = l; };
    inline void setLon(float l) { lon_ = l; };
    inline void setPrecipitation(float p) { precipitation_ = p; };
    inline void setFog(float f) { fog_ = f; };
    inline void setCloudiness(int c) { cloudiness_ = c; };
    inline void setWindSpeed(int w) { windSpeed_ = w; };
    inline void setMaxTemp(int m) { maxTemp_ = m; };
    inline void setMinTemp(int m) { minTemp_ = m; };
    inline void setHumidity(int h) { humidity_ = h; };
    inline void setPressure(int p) { pressure_ = p; };

private:
    QString locationName_;
    QString windDirection_;
    QString weatherDescription_;
    QString weatherIcon_;
    QDateTime time_;
    float lat_;
    float lon_;
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
