#include "weatherhourmodel.h"
#include <QQmlEngine>

/* ~~~ WeatherHour ~~~ */

WeatherHour::WeatherHour()
{
}

WeatherHour::WeatherHour(AbstractWeatherForecast *forecast)
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
    this->date_ = QDateTime(forecast->time().date(), QTime(forecast->time().time().hour(), 0));
}

/* ~~~ WeatherHourListModel ~~~ */

WeatherHourListModel::WeatherHourListModel(WeatherLocation *location)
{
    connect(location, &WeatherLocation::weatherRefresh, this, &WeatherHourListModel::refreshHoursFromForecasts);
}

int WeatherHourListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return dayList.at(day + 1) - dayList.at(day);
}

QVariant WeatherHourListModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

WeatherHour *WeatherHourListModel::get(int index)
{
    WeatherHour *ret;
    if (index < 0 || index >= hoursList.count()) {
        return {};
    } else {
        ret = hoursList.at(index + dayList.at(day));
    }
    // it's kind of dumb how much seems to be garbage collected by js...
    // this fixes segfaults with scrolling with the hour view
    QQmlEngine::setObjectOwnership(ret, QQmlEngine::CppOwnership);
    return ret;
}

void WeatherHourListModel::refreshHoursFromForecasts(QList<AbstractWeatherForecast *> forecasts)
{
    // clear forecasts
    emit layoutAboutToBeChanged();
    day = 0;
    hoursList.clear();
    dayList.clear();
    dayList.append(0);
    // insert forecasts
    int currentDay = forecasts.at(0)->time().date().day();
    int i = 0;
    for (auto forecast : forecasts) {
        if (currentDay != forecast->time().date().day()) {
            currentDay = forecast->time().date().day();
            dayList.append(i);
        }
        auto *weatherHour = new WeatherHour(forecast);
        QQmlEngine::setObjectOwnership(weatherHour, QQmlEngine::CppOwnership); // prevent segfaults from js garbage collecting
        hoursList.append(weatherHour);
        i++;
    }
    dayList.append(i);
    std::sort(hoursList.begin(), hoursList.end(), [](WeatherHour *h1, WeatherHour *h2) -> bool { return h1->date() < h2->date(); });
    for (auto i : dayList) {
        qDebug() << i;
    }
    emit layoutChanged();
}

void WeatherHourListModel::updateUi(int index)
{
    emit layoutAboutToBeChanged();
    day = index;
    emit layoutChanged();
}
