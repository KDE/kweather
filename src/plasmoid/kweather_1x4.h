/*
    SPDX-FileCopyrightText: 2020 HanY <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef KWEATHER_1X4_H
#define KWEATHER_1X4_H

#include <Plasma/Applet>

#include <KWeatherCore/WeatherForecastSource>

class HourlyModel;
class QTimer;
class KWeather_1x4 : public Plasma::Applet
{
    Q_OBJECT
    Q_PROPERTY(bool needLocation READ needLocation NOTIFY needLocationChanged)
    Q_PROPERTY(QString location READ location NOTIFY locationChanged)
    Q_PROPERTY(qreal temp READ temp NOTIFY updated)
    Q_PROPERTY(QString desc READ desc NOTIFY updated)
    Q_PROPERTY(QString weatherIcon READ weatherIcon NOTIFY updated)
    Q_PROPERTY(qreal humidity READ humidity NOTIFY updated)
    Q_PROPERTY(qreal precipitation READ precipitation NOTIFY updated)
    Q_PROPERTY(HourlyModel *hourlyModel READ hourlyModel NOTIFY hourlyModelChanged)
public:
    KWeather_1x4(QObject *parent, const KPluginMetaData &md, const QVariantList &args);
    QString location() const;
    QString desc() const;
    qreal temp() const;
    QString weatherIcon() const;
    qreal humidity() const;
    qreal precipitation() const;
    bool needLocation() const
    {
        return m_needLocation;
    }
    HourlyModel *hourlyModel() const
    {
        return m_hourlyModel;
    }

    Q_INVOKABLE QStringList locationsInSystem();
    Q_INVOKABLE void setLocation(const QString &location);
Q_SIGNALS:
    void locationChanged();
    void updated();
    void needLocationChanged();
    void hourlyModelChanged();

private:
    void update();
    bool hasForecast() const;
    const KWeatherCore::HourlyWeatherForecast &getFirst() const;

    bool m_needLocation = true;
    QString m_location;
    double m_latitude, m_longitude;
    KWeatherCore::WeatherForecast m_forecast;
    KWeatherCore::WeatherForecastSource m_source;

    HourlyModel *m_hourlyModel = nullptr;
    QTimer *m_timer = nullptr;
};

#endif
