#include "weatherdaymodel.h"

/* ~~~ WeatherDay ~~~ */

WeatherDay::WeatherDay() {}

WeatherDay::WeatherDay(QList<AbstractWeatherForecast*> forecasts)
{

    int minTemp = 1e9, maxTemp = -1e9;
    for (auto forecast : forecasts) {
        minTemp = std::min(minTemp, forecast->minTemp());
        maxTemp = std::max(maxTemp, forecast->maxTemp());

        this->date_ = forecast->time().date();
        this->weatherDescription_ = forecast->weatherDescription();
        this->weatherIcon_ = forecast->weatherIcon();
    }

    if (minTemp != 1e9) this->minTemp_ = minTemp;
    if (maxTemp != -1e9) this->maxTemp_ = maxTemp;
}

/* ~~~ WeatherHourListModel ~~~ */

WeatherDayListModel::WeatherDayListModel(WeatherLocation* location)
{
    connect(location, &WeatherLocation::weatherRefresh, this, &WeatherDayListModel::refreshDaysFromForecasts);
}

int WeatherDayListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return daysList.size();
}

QVariant WeatherDayListModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
}

void WeatherDayListModel::refreshDaysFromForecasts(QList<AbstractWeatherForecast*> forecasts) 
{
    emit beginRemoveRows(QModelIndex(), 0, daysList.count() - 1);
    daysList.clear();
    emit endRemoveRows();

    emit beginInsertRows(QModelIndex(), 0, forecasts.count() - 1);
    std::map<QPair<int, int>, QList<AbstractWeatherForecast*>> forecastMap;

    // order forecasts into lists by day
    for (auto forecast : forecasts) {
        QPair<int, int> dayMonthPair = QPair<int, int>(forecast->time().date().day(), forecast->time().date().month());
        auto d = forecastMap.find(dayMonthPair);
        if (d == forecastMap.end()) {
            forecastMap.insert(std::make_pair(dayMonthPair, QList<AbstractWeatherForecast*>({forecast})));
        } else {
            d->second.append(forecast);
        }
    }

    // add weatherdays with forecast day lists
    for (const auto& mapPair : forecastMap) {
        daysList.append(new WeatherDay(mapPair.second));
    }
    emit endInsertRows();
}

void WeatherDayListModel::updateUi() {}
