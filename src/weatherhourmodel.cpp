#include "weatherhourmodel.h"
#include <QQmlEngine>

/* ~~~ WeatherHour ~~~ */

WeatherHour::WeatherHour() {}

WeatherHour::WeatherHour(AbstractWeatherForecast* forecast)
{
    this->windDirection_ = forecast->windDirection();
    this->weatherDescription_ = forecast->weatherDescription();
    this->weatherIcon_ = forecast->weatherIcon();
    this->precipitation_ = forecast->precipitation();
    this->fog_ = forecast->fog();
    this->cloudiness_ = forecast->cloudiness();
    this->windSpeed_ = forecast->windSpeed();
    this->temperature_ = forecast->maxTemp(); // TODO
    this->humidity_ = forecast->humidity();
    this->pressure_ = forecast->pressure();
    this->hour_ = forecast->time().time().hour();
    this->day_ = forecast->time().date().day();
}

/* ~~~ WeatherHourListModel ~~~ */

WeatherHourListModel::WeatherHourListModel(WeatherLocation* location)
{
    connect(location, &WeatherLocation::weatherRefresh, this, &WeatherHourListModel::refreshHoursFromForecasts);
}

int WeatherHourListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return hoursList.size();
}

QVariant WeatherHourListModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
}

WeatherHour* WeatherHourListModel::get(int index) {
    WeatherHour* ret;
    if (index < 0 || index >= hoursList.count()) {
        ret = new WeatherHour();
    } else {
        ret = hoursList.at(index);
    }
    // it's kind of dumb how much seems to be garbage collected by js...
    // this fixes segfaults with scrolling with the hour view
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::CppOwnership);
    return ret;
}

void WeatherHourListModel::refreshHoursFromForecasts(QList<AbstractWeatherForecast*> forecasts) 
{
    // clear forecasts
    emit beginRemoveRows(QModelIndex(), 0, hoursList.count()-1);
    hoursList.clear();
    emit endRemoveRows();
    
    // insert forecasts
    emit beginInsertRows(QModelIndex(), 0, forecasts.count() - 1);
    
    for (auto forecast : forecasts) {
        WeatherHour* weatherHour = new WeatherHour(forecast);
        QQmlEngine::setObjectOwnership(weatherHour, QQmlEngine::CppOwnership); // prevent segfaults from js garbage collecting
        hoursList.append(new WeatherHour(forecast));
    }
    std::sort(hoursList.begin(), hoursList.end(), [](WeatherHour* h1, WeatherHour* h2)->bool{ return h1->hour() < h2->hour(); });

    emit endInsertRows();
}

void WeatherHourListModel::updateUi() {}
