/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "temperaturechartdata.h"

#include <QDateTimeAxis>
#include <QPointF>
#include <QSplineSeries>
#include <QValueAxis>

#include "global.h"

QVariantList TemperatureChartData::weatherData() const
{
    return m_weatherData;
}

void TemperatureChartData::setWeatherData(const QVariantList &weatherData)
{
    m_weatherData = weatherData;
    Q_EMIT weatherDataChanged();

    updateData();
}

void TemperatureChartData::updateData()
{
    if (!m_series) {
        return;
    }

    m_series->clear();

    QList<QPointF> result;
    result.reserve(m_weatherData.size());

    double minTemp = std::numeric_limits<double>::max();
    double maxTemp = std::numeric_limits<double>::min();

    for (const QVariant &dayVariant : std::as_const(m_weatherData)) {
        const auto day = dayVariant.value<KWeatherCore::DailyWeatherForecast>();

        const double dayMinTemp = KWeather::convertTemp(day.minTemp(), KWeatherSettings::self()->temperatureUnits());
        const double dayMaxTemp = KWeather::convertTemp(day.maxTemp(), KWeatherSettings::self()->temperatureUnits());

        result.append(QPointF(day.date().startOfDay().toMSecsSinceEpoch(), dayMaxTemp));
        minTemp = std::min<double>(dayMinTemp, minTemp);
        maxTemp = std::max<double>(dayMaxTemp, maxTemp);
    }

    // make enough room for the curve
    m_maxTempLimit = maxTemp + 5;
    m_minTempLimit = minTemp - 5;

    Q_EMIT maxTempLimitChanged();
    Q_EMIT minTempLimitChanged();

    m_series->replace(result);

    if (!m_weatherData.isEmpty()) {
        m_axisX->setRange(m_weatherData.first().value<KWeatherCore::DailyWeatherForecast>().date().startOfDay(),
                          m_weatherData.last().value<KWeatherCore::DailyWeatherForecast>().date().startOfDay());
    }
}

void TemperatureChartData::initSeries(QAbstractSeries *series)
{
    m_series = static_cast<QSplineSeries *>(series);
    updateData();
}

void TemperatureChartData::initAxes(QObject *axisX, QObject *axisY)
{
    m_axisX = static_cast<QDateTimeAxis *>(axisX);
    m_axisY = static_cast<QValueAxis *>(axisY);
}

double TemperatureChartData::maxTempLimit() const
{
    return m_maxTempLimit;
}
double TemperatureChartData::minTempLimit() const
{
    return m_minTempLimit;
}

#include "moc_temperaturechartdata.cpp"
