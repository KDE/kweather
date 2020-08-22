/*
    SPDX-FileCopyrightText: 2020 HanY <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

#ifndef KWEATHER_1X4_H
#define KWEATHER_1X4_H
#include <Plasma/Applet>
#include <QAbstractListModel>
class QDBusInterface;
class KWeather_1x4 : public Plasma::Applet
{
    Q_OBJECT
    Q_PROPERTY(QStringList date READ date NOTIFY dateChanged)
    Q_PROPERTY(QStringList forecast READ forecast NOTIFY dataUpdated)
    Q_PROPERTY(QStringList maxMinTemp READ maxMinTemp NOTIFY dataUpdated)
    Q_PROPERTY(QString cityName READ cityName NOTIFY locationChanged)
    Q_PROPERTY(QString description READ description NOTIFY dataUpdated)
    Q_PROPERTY(QString tempNow READ tempNow NOTIFY dataUpdated)
public:
    KWeather_1x4(QObject *parent, const QVariantList &args);
    QStringList date()
    {
        return m_date;
    };
    QStringList forecast()
    {
        return m_forecast;
    };
    QStringList maxMinTemp()
    {
        return m_maxMinTemp;
    };
    QString cityName()
    {
        return m_cityName;
    };
    QString description()
    {
        return m_description;
    };
    QString tempNow()
    {
        return m_tempNow;
    };
signals:
    void dateChanged();
    void dataUpdated();
    void locationChanged();
private slots:
    void addCity(QString locationID);
    void removeCity(QString locationID);
    void update();

private:
    QDBusInterface *m_interface;

    void parse(QJsonDocument);

    QStringList m_date;
    QStringList m_forecast;
    QStringList m_maxMinTemp;
    QString m_cityName;
    QString m_description;
    QString m_tempNow;
    QList<QString> locationList;
};

#endif
