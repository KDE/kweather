#include "abstractdailyweatherforecast.h"
#include <QtCore/QJsonObject>

AbstractDailyWeatherForecast::AbstractDailyWeatherForecast()
{
}

AbstractDailyWeatherForecast::AbstractDailyWeatherForecast(float maxTemp, float minTemp, float precipitation, QString weatherIcon, QString weatherDescription, QDate date)
    : maxTemp_(maxTemp)
    , minTemp_(minTemp)
    , precipitation_(precipitation)
    , weatherIcon_(weatherIcon)
    , weatherDescription_(weatherDescription)
    , date_(date)
{
}

AbstractDailyWeatherForecast* AbstractDailyWeatherForecast::fromJson(QJsonObject obj)
{
    auto* fc = new AbstractDailyWeatherForecast();
    fc->setMaxTemp(obj["maxTemp"].toDouble());
    fc->setMinTemp(obj["minTemp"].toDouble());
    fc->setPrecipitation(obj["precipitation"].toDouble());
    fc->setWeatherIcon(obj["weatherIcon"].toString());
    fc->setWeatherDescription(obj["weatherDescription"].toString());
    fc->setDate(QDate::fromString(obj["date"].toString(), Qt::ISODate));
    return fc;
}

QJsonObject AbstractDailyWeatherForecast::toJson()
{
    QJsonObject obj;
    obj[QLatin1String("maxTemp")] = maxTemp();
    obj[QLatin1String("minTemp")] = minTemp();
    obj[QLatin1String("precipitation")] = precipitation();
    obj[QLatin1String("weatherIcon")] = weatherIcon();
    obj[QLatin1String("weatherDescription")] = weatherDescription();
    obj[QLatin1String("date")] = date().toString(Qt::ISODate);
    return obj;
}