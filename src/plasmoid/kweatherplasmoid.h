/*
 * SPDX-FileCopyrightText: 2020 HanY <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2021 Devin Lin <devin@kde.org>
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#pragma once

#include <Plasma/Applet>

#include <KWeatherCore/WeatherForecast>

class KWeatherPlasmoid : public Plasma::Applet
{
    Q_OBJECT
    Q_PROPERTY(bool needLocation READ needLocation NOTIFY needLocationChanged)

    Q_PROPERTY(QString locationName READ location NOTIFY locationChanged)
    Q_PROPERTY(QString currentTemperature READ currentTemperature NOTIFY updated)
    Q_PROPERTY(QString currentDescription READ currentDescription NOTIFY updated)
    Q_PROPERTY(QString weatherIcon READ weatherIcon NOTIFY updated)
    Q_PROPERTY(HourlyModel *hourlyModel READ hourlyModel NOTIFY hourlyModelChanged)

public:
    KWeatherPlasmoid(QObject *parent, const QVariantList &args);

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

    HourlyModel *m_hourlyModel;
};
