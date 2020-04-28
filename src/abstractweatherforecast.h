#ifndef ABSTRACTWEATHERFORECAST_H
#define ABSTRACTWEATHERFORECAST_H
#include <QDateTime>

class AbstractWeatherforecast
{
public:
    AbstractWeatherforecast(QString LocationName,
                            QString WindDirection,
                            QString WeatherDescription,
                            QString WeatherIcon,
                            QDateTime Time,
                            float Lat,
                            float Lon,
                            float Precipitation,
                            float Fog,
                            int Clouldiness,
                            int WindSpeed,
                            int MaxTemp,
                            int MinTemp,
                            int Humidity,
                            int Pressure
                            );
    const QString& locationName();
    const QString& windDirection();
    const QString& weatherDescription();
    const QString& weatherIcon();
    inline const QDateTime time(){return time_;};
    inline float lat(){return lat_;};
    inline float lon(){return lon_;};
    inline float precipitation(){return precipitation_;};
    inline float fog(){return fog_;};
    inline int clouldiness(){return clouldiness_;};
    inline int windSpeed(){return windSpeed_;};
    inline int maxTemp(){return maxTemp_;};
    inline int minTemp(){return minTemp_;};
    inline int humidity(){return humidity_;};
    inline int pressure(){return pressure_;};
private:
    const QString locationName_;
    const QString windDirection_;
    const QString weatherDescription_;
    const QString weatherIcon_;
    const QDateTime time_;
    const float lat_;
    const float lon_;
    const float precipitation_;
    const float fog_;
    const int clouldiness_;
    const int windSpeed_;
    const int maxTemp_;
    const int minTemp_;
    const int humidity_;
    const int pressure_;
};

#endif // ABSTRACTWEATHERFORECAST_H
