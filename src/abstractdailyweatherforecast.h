#ifndef ABSTRACTDAILYWEATHERFORECAST_H
#define ABSTRACTDAILYWEATHERFORECAST_H
#include <QDate>

class AbstractDailyWeatherForecast
{
public:
    AbstractDailyWeatherForecast();
    AbstractDailyWeatherForecast(float maxTemp, float minTemp, float precipitation, QString weatherIcon, QString weatherDescription, QDate date);
    inline void setMaxTemp(float maxTemp)
    {
        maxTemp_ = maxTemp;
    };
    inline void setMinTemp(float minTemp)
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
    inline void setDate(QDate date)
    {
        date_ = date;
    };
    inline float maxTemp() const
    {
        return maxTemp_;
    };
    inline float minTemp() const
    {
        return minTemp_;
    };
    inline float precipitation() const
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
    inline QDate date()
    {
        return date_;
    };

private:
    float maxTemp_{};
    float minTemp_{};
    float precipitation_{};
    QString weatherIcon_;
    QString weatherDescription_;
    QDate date_;
};

#endif // ABSTRACTDAILYWEATHERFORECAST_H
