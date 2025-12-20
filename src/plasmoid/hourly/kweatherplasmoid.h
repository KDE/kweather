/*
    SPDX-FileCopyrightText: 2020 HanY <hanyoung@protonmail.com>
    SPDX-FileCopyrightText: 2025 Devin Lin <devin@kde.org>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#pragma once

#include <Plasma/Applet>

#include "locationquerymodel.h"
#include <KWeatherCore/WeatherForecastSource>

class HourlyModel;
class QTimer;
class KWeatherPlasmoid : public Plasma::Applet
{
    Q_OBJECT
    Q_PROPERTY(bool needLocation READ needLocation NOTIFY needLocationChanged)
    Q_PROPERTY(QString locationName READ locationName NOTIFY weatherLocationChanged)
    Q_PROPERTY(qreal temp READ temp NOTIFY updated)
    Q_PROPERTY(QString desc READ desc NOTIFY updated)
    Q_PROPERTY(QString weatherIcon READ weatherIcon NOTIFY updated)
    Q_PROPERTY(qreal humidity READ humidity NOTIFY updated)
    Q_PROPERTY(qreal precipitation READ precipitation NOTIFY updated)
    Q_PROPERTY(QColor topColor READ topColor NOTIFY updated)
    Q_PROPERTY(QColor bottomColor READ bottomColor NOTIFY updated)
    Q_PROPERTY(HourlyModel *hourlyModel READ hourlyModel NOTIFY hourlyModelChanged)
    Q_PROPERTY(LocationQueryModel *queryModel READ queryModel CONSTANT)

public:
    KWeatherPlasmoid(QObject *parent, const KPluginMetaData &md, const QVariantList &args);
    QString locationName() const;
    QString desc() const;
    qreal temp() const;
    QString weatherIcon() const;
    qreal humidity() const;
    qreal precipitation() const;
    QColor topColor() const;
    QColor bottomColor() const;
    bool needLocation() const;
    HourlyModel *hourlyModel() const;
    LocationQueryModel *queryModel() const;

    Q_INVOKABLE void setLocation(const QString &locationName, double latitude, double longitude);

Q_SIGNALS:
    void weatherLocationChanged();
    void updated();
    void needLocationChanged();
    void hourlyModelChanged();

private:
    void update();
    bool hasForecast() const;
    const KWeatherCore::HourlyWeatherForecast &getFirst() const;

    bool m_needLocation = true;
    QString m_locationName;
    double m_latitude;
    double m_longitude;

    KWeatherCore::WeatherForecast m_forecast;
    KWeatherCore::WeatherForecastSource m_source;

    HourlyModel *m_hourlyModel = nullptr;
    LocationQueryModel *m_queryModel = nullptr;
    QTimer *m_timer = nullptr;
};
