/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <QApplication>
#include <QMetaObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QUrl>
#include <QtQml>

#include <KAboutData>

#ifndef Q_OS_ANRDOID
#include <KDBusService>
#endif

#include <KLocalizedContext>
#include <KLocalizedString>

#include "abstractdailyweatherforecast.h"
#include "abstracthourlyweatherforecast.h"
#include "kweathersettings.h"
#include "locationquerymodel.h"
#include "weatherdaymodel.h"
#include "weatherforecastmanager.h"
#include "weatherhourmodel.h"
#include "weatherlocationmodel.h"

class AbstractHourlyWeatherForecast;
class AbstractDailyWeatherForecast;

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    KLocalizedString::setApplicationDomain("kweather");
    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    KAboutData aboutData("kweather", i18n("Weather"), "0.2", i18n("Weather application in Kirigami"), KAboutLicense::GPL, i18n("© 2020 KDE Community"));
    KAboutData::setApplicationData(aboutData);

#ifndef Q_OS_ANRDOID
    // only allow one instance
    KDBusService service(KDBusService::Unique);
#endif

    // allow to stay running when last window is closed
    app.setQuitOnLastWindowClosed(false);

    // initialize models in context
    auto *weatherLocationListModel = new WeatherLocationListModel();
    auto *locationQueryModel = new LocationQueryModel();
    WeatherForecastManager::instance(*weatherLocationListModel);

    KWeatherSettings settings;

    engine.rootContext()->setContextProperty("weatherLocationListModel", weatherLocationListModel);
    engine.rootContext()->setContextProperty("locationQueryModel", locationQueryModel);
    engine.rootContext()->setContextProperty("settingsModel", &settings);
    // the longer the merrier, this add locations
    QObject::connect(locationQueryModel, &LocationQueryModel::appendLocation, [weatherLocationListModel, locationQueryModel] { weatherLocationListModel->addLocation(locationQueryModel->get(locationQueryModel->index_)); });

    // register QML types
    qmlRegisterType<WeatherLocation>("kweather", 1, 0, "WeatherLocation");
    qmlRegisterType<WeatherDay>("kweather", 1, 0, "WeatherDay");
    qmlRegisterType<WeatherHour>("kweather", 1, 0, "WeatherHour");
    qmlRegisterType<WeatherHourListModel>("kweather", 1, 0, "WeatherHourListModel");
    qmlRegisterType<WeatherDayListModel>("kweather", 1, 0, "WeatherDayListModel");

    engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    QObject *rootObject = engine.rootObjects().first();

#ifndef Q_OS_ANRDOID
    QObject::connect(&service, &KDBusService::activateRequested, rootObject, [=](const QStringList &arguments, const QString &workingDirectory) {
        Q_UNUSED(workingDirectory)
        QMetaObject::invokeMethod(rootObject, "show");
    });
#endif

    return app.exec();
}
