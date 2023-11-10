/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <KWeatherCore/DailyWeatherForecast>

#include <QtGlobal>
#include <qqmlregistration.h>

class QAbstractSeries;
class QSplineSeries;
class QDateTimeAxis;
class QValueAxis;

class TemperatureChartData : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QVariantList weatherData READ weatherData WRITE setWeatherData NOTIFY weatherDataChanged)
    Q_PROPERTY(double maxTempLimit READ maxTempLimit NOTIFY maxTempLimitChanged)
    Q_PROPERTY(double minTempLimit READ minTempLimit NOTIFY minTempLimitChanged)

public:
    QVariantList weatherData() const;
    void setWeatherData(const QVariantList &weatherData);

    double maxTempLimit() const;
    double minTempLimit() const;

    Q_INVOKABLE void initSeries(QAbstractSeries *series);
    Q_INVOKABLE void initAxes(QObject *axisX, QObject *axisY);

Q_SIGNALS:
    void weatherDataChanged();
    void maxTempLimitChanged();
    void minTempLimitChanged();

private:
    void updateData();

    QVariantList m_weatherData;
    double m_maxTempLimit = 100;
    double m_minTempLimit = 0;

    QSplineSeries *m_series = nullptr;
    QDateTimeAxis *m_axisX = nullptr;
    QValueAxis *m_axisY = nullptr;
};
