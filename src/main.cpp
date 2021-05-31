/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include <QMetaObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickStyle>
#include <QUrl>
#include <QtQml>

#ifdef Q_OS_ANDROID
#include <QGuiApplication>
#else
#include <QApplication>
#endif

#include <KAboutData>
#include <KConfigCore/KConfig>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "kweathersettings.h"
#include "locationquerymodel.h"
#include "weatherdaymodel.h"
#include "weatherforecastmanager.h"
#include "weatherhourmodel.h"
#include "weatherlocation.h"
#include "weatherlocationmodel.h"
class AbstractHourlyWeatherForecast;
class AbstractDailyWeatherForecast;

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

#ifdef Q_OS_ANDROID
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle(QStringLiteral("Material"));
#else
    QApplication app(argc, argv);
    if(qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }
#endif
    QQmlApplicationEngine engine;

    KLocalizedString::setApplicationDomain("kweather");
    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    KAboutData aboutData("kweather", i18n("Weather"), "21.05", i18n("A convergent weather application for Plasma"), KAboutLicense::GPL, i18n("© 2020-2021 Plasma Development Team"));
    aboutData.addAuthor(i18n("Han Young"), QString(), QStringLiteral("hanyoung@protonmail.com"));
    aboutData.addAuthor(i18n("Devin Lin"), QString(), QStringLiteral("espidev@gmail.com"), QStringLiteral("https://espi.dev"));
    KAboutData::setApplicationData(aboutData);

    engine.rootContext()->setContextProperty(QStringLiteral("kweatherAboutData"), QVariant::fromValue(aboutData));

    // initialize models in context
    auto *locationQueryModel = new LocationQueryModel();
    QObject::connect(locationQueryModel, &LocationQueryModel::appendLocation, WeatherForecastManager::inst()->model(), &WeatherLocationListModel::addLocation);
    KWeatherSettings settings;

    engine.rootContext()->setContextProperty("weatherLocationListModel", WeatherForecastManager::inst()->model());
    engine.rootContext()->setContextProperty("settingsModel", &settings);
    engine.rootContext()->setContextProperty("locationQueryModel", locationQueryModel);

    // register QML types
    qmlRegisterType<WeatherLocation>("kweather", 1, 0, "WeatherLocation");
    qmlRegisterType<WeatherDay>("kweather", 1, 0, "WeatherDay");
    qmlRegisterType<WeatherHour>("kweather", 1, 0, "WeatherHour");
    qmlRegisterType<WeatherHourListModel>("kweather", 1, 0, "WeatherHourListModel");
    qmlRegisterType<WeatherDayListModel>("kweather", 1, 0, "WeatherDayListModel");

    // load setup wizard if first launch
    engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
