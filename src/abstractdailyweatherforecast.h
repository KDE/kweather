#ifndef ABSTRACTDAILYWEATHERFORECAST_H
#define ABSTRACTDAILYWEATHERFORECAST_H
#include <QDate>

class AbstractDailyWeatherForecast
{
public:
    AbstractDailyWeatherForecast();
    AbstractDailyWeatherForecast(int maxTemp, int minTemp, float precipitation, QString weatherIcon, QString weatherDescription, QDateTime date);
    inline void setMaxTemp(int maxTemp)
    {
        maxTemp_ = maxTemp;
    };
    inline void setMinTemp(int minTemp)
    {
        minTemp_ = minTemp;
    };
    inline void setPrecipitation(float precipitation)
    {
        precipitation_ = precipitation;
    };
    inline void setWeatherIcon(QString icon)
    {
        weatherIcon_ = icon;
    };
    inline void setWeatherDescription(QString description)
    {
        weatherDescription_ = description;
    };
    inline void setDate(QDateTime date)
    {
        date_ = date;
    };
    inline int maxTemp()
    {
        return maxTemp_;
    };
    inline int minTemp()
    {
        return minTemp_;
    };
    inline float precipitation()
    {
        return precipitation_;
    };
    inline QString &weatherIcon()
    {
        return weatherIcon_;
    };
    inline QString &weatherDescription()
    {
        return weatherDescription_;
    };
    inline QDateTime date()
    {
        return date_;
    };

private:
    int maxTemp_;
    int minTemp_;
    float precipitation_;
    QString weatherIcon_;
    QString weatherDescription_;
    QDateTime date_;
};

#endif // ABSTRACTDAILYWEATHERFORECAST_H
