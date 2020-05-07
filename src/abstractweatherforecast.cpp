#include "abstractweatherforecast.h"
#include <QDebug>
#include <limits>
#include <utility>
AbstractWeatherForecast::AbstractWeatherForecast(QDateTime timeCreated)
{
    this->timeCreated_ = timeCreated;
}
AbstractWeatherForecast::AbstractWeatherForecast(QDateTime timeCreated, QString locationName, float latitude, float longitude, QList<AbstractHourlyWeatherForecast *> hourlyForecasts, QList<AbstractDailyWeatherForecast *> dailyForecasts)
    : timeCreated_(timeCreated)
    , locationName_(std::move(locationName))
    , latitude_(latitude)
    , longitude_(longitude)
    , hourlyForecasts_(hourlyForecasts)
    , dailyForecasts_(dailyForecasts)
{
}

AbstractWeatherForecast::~AbstractWeatherForecast()
{
    for (auto hour : hourlyForecasts_)
        delete hour;
    for (auto day : dailyForecasts_)
        delete day;
}

void AbstractWeatherForecast::sortDailyForecast()
{
    // insert sort, because we only have 9 items to sort, reduce overhead
    int i;
    AbstractDailyWeatherForecast *k;
    for (int j = 2; j < this->dailyForecasts().count(); j++) {
        k = this->dailyForecasts().at(j);
        i = j - 1;
        while (i >= 0 && this->dailyForecasts().at(i)->date().daysTo(k->date()) < 0) {
            this->dailyForecasts_.replace(i + 1, this->dailyForecasts_.at(i));
            i--;
        }
        this->dailyForecasts_.replace(i + 1, k);
    }
}
