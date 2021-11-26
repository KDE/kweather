/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#pragma once

#include <QAbstractListModel>
#include <QColor>
#include <QDateTime>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QTimeZone>
#include <QTimer>

#include <KWeatherCore/WeatherForecastSource>

#include <utility>

namespace QtCharts
{
class QAbstractSeries;
class QSplineSeries;
class QDateTimeAxis;
class QValueAxis;
}

class WeatherLocation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ locationName CONSTANT)
    Q_PROPERTY(QString lastUpdated READ lastUpdatedFormatted NOTIFY lastUpdatedChanged)
    Q_PROPERTY(QString currentTime READ currentTimeFormatted NOTIFY currentTimeChanged)
    Q_PROPERTY(QString currentDate READ currentDateFormatted NOTIFY currentDateChanged)
    Q_PROPERTY(QVariantList dayForecasts READ dayForecasts NOTIFY dayForecastsChanged)
    Q_PROPERTY(QVariantList hourForecasts READ hourForecasts NOTIFY hourForecastsChanged)
    Q_PROPERTY(int selectedDay READ selectedDay WRITE setSelectedDay NOTIFY selectedDayChanged)

    Q_PROPERTY(QString backgroundComponent READ backgroundComponent NOTIFY currentForecastChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor NOTIFY currentForecastChanged)
    Q_PROPERTY(QColor textColor READ textColor NOTIFY currentForecastChanged)
    Q_PROPERTY(QColor cardBackgroundColor READ cardBackgroundColor NOTIFY currentForecastChanged)
    Q_PROPERTY(QColor cardTextColor READ cardTextColor NOTIFY currentForecastChanged)
    Q_PROPERTY(QColor cardSecondaryTextColor READ cardSecondaryTextColor NOTIFY currentForecastChanged)
    Q_PROPERTY(QColor iconColor READ iconColor NOTIFY currentForecastChanged)
    Q_PROPERTY(bool darkTheme READ darkTheme NOTIFY currentForecastChanged)

public:
    explicit WeatherLocation(QString locationId,
                             QString locationName,
                             QString timeZone,
                             float latitude = 0,
                             float longitude = 0,
                             KWeatherCore::WeatherForecast forecast = {});
    explicit WeatherLocation(); // when creating empty locations, don't persist

    void save();
    static WeatherLocation *load(const QString &groupName);
    Q_INVOKABLE void update();

    const QString &locationId() const
    {
        return m_locationId;
    }
    const QString &locationName() const
    {
        return m_locationName;
    }
    const QString &timeZone() const
    {
        return m_timeZone;
    };
    float latitude() const
    {
        return m_latitude;
    }
    float longitude() const
    {
        return m_longitude;
    }
    QString lastUpdatedFormatted() const
    {
        return lastUpdated().toString(QStringLiteral("hh:mm ap"));
    }
    const QDateTime &lastUpdated() const
    {
        return m_lastUpdated;
    }
    QString currentTimeFormatted() const
    {
        return currentDateTime().toString(QStringLiteral("hh:mm ap"));
    }
    QString currentDateFormatted() const
    {
        return currentDateTime().toString(QStringLiteral("dd MMM yyyy"));
    }
    QDateTime currentDateTime() const
    {
        return QDateTime::currentDateTime().toTimeZone(QTimeZone(m_timeZone.toUtf8()));
    }
    const QString &backgroundComponent() const
    {
        return m_backgroundComponent;
    };
    const QColor &backgroundColor() const
    {
        return m_backgroundColor;
    };
    const QColor &textColor() const
    {
        return m_textColor;
    };
    const QColor &cardBackgroundColor() const
    {
        return m_cardBackgroundColor;
    };
    const QColor &cardTextColor() const
    {
        return m_cardTextColor;
    };
    const QColor &cardSecondaryTextColor() const
    {
        return m_cardSecondaryTextColor;
    }

    const QColor &iconColor() const
    {
        return m_iconColor;
    }
    bool darkTheme() const
    {
        return m_isDarkTheme;
    }
    QVariantList dayForecasts() const
    {
        return m_dayForecasts;
    }
    QVariantList hourForecasts() const
    {
        return m_hourForecasts;
    }
    int selectedDay() const
    {
        return m_selectedDay;
    }
    void setSelectedDay(int selectedDay)
    {
        if (selectedDay != m_selectedDay) {
            m_selectedDay = selectedDay;
            Q_EMIT selectedDayChanged();
        }
    }

    // for restore order of locations
    void saveOrder(int index);
    int index();
    void deleteConfig();
public Q_SLOTS:
    void updateData(KWeatherCore::WeatherForecast forecasts);

Q_SIGNALS:
    void weatherRefresh(KWeatherCore::WeatherForecast forecasts); // sent when weather data is refreshed
    void currentForecastChanged();
    void stopLoadingIndicator();
    void currentTimeChanged();
    void currentDateChanged();
    void dayForecastsChanged();
    void hourForecastsChanged();
    void selectedDayChanged();
    void lastUpdatedChanged();

    void chartListChanged();
private Q_SLOTS:
    void updateCurrentDateTime();

private:
    void determineCurrentForecast();

    KWeatherCore::WeatherForecastSource m_source;
    KWeatherCore::WeatherForecast m_forecast;

    // background related fields
    QColor m_backgroundColor;
    QColor m_textColor;
    QColor m_cardBackgroundColor;
    QColor m_cardTextColor;
    QColor m_cardSecondaryTextColor;
    QColor m_iconColor;
    QString m_backgroundComponent = QStringLiteral("backgrounds/ClearDay.qml");
    bool m_isDarkTheme = false;

    QString m_locationName, m_locationId;
    QString m_timeZone;
    QDateTime m_lastUpdated;
    QTimer *m_timer;
    float m_latitude, m_longitude;

    QVariantList m_dayForecasts;
    QVariantList m_hourForecasts;
    int m_selectedDay = 0;
};
