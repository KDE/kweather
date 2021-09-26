/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
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

#include "formatter.h"
#include "kweathersettings.h"
#include "locationquerymodel.h"
#include "temperaturechartdata.h"
#include "about.h"
#include "version.h"
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
    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }
#endif
    QQmlApplicationEngine engine;

    KLocalizedString::setApplicationDomain("kweather");
    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    KAboutData aboutData(QStringLiteral("kweather"),
                         i18n("Weather"),
                         QStringLiteral(KWEATHER_VERSION_STRING),
                         i18n("A convergent weather application for Plasma"),
                         KAboutLicense::GPL,
                         i18n("© 2020-2021 Plasma Development Team"));
    aboutData.addAuthor(i18n("Han Young"), QString(), QStringLiteral("hanyoung@protonmail.com"));
    aboutData.addAuthor(i18n("Devin Lin"), QString(), QStringLiteral("espidev@gmail.com"), QStringLiteral("https://espi.dev"));
    KAboutData::setApplicationData(aboutData);

    qmlRegisterSingletonType<AboutType>("kweather", 1, 0, "AboutType", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)

        return new AboutType();
    });

    engine.rootContext()->setContextProperty(QStringLiteral("weatherLocationListModel"), WeatherLocationListModel::inst());
    engine.rootContext()->setContextProperty(QStringLiteral("settingsModel"), KWeatherSettings::self());

    WeatherLocation *emptyWeatherLocation = new WeatherLocation();
    engine.rootContext()->setContextProperty(QStringLiteral("emptyWeatherLocation"), emptyWeatherLocation);

    Formatter formatter;
    qmlRegisterSingletonInstance<Formatter>("kweather", 1, 0, "Formatter", &formatter);

    qmlRegisterType<TemperatureChartData>("kweather", 1, 0, "TemperatureChartData");
    qmlRegisterType<LocationQueryModel>("kweather", 1, 0, "LocationQueryModel");
    qmlRegisterType<WeatherLocation>("kweather", 1, 0, "WeatherLocation");

    qRegisterMetaType<KWeatherCore::Sunrise>();
    qRegisterMetaType<KWeatherCore::HourlyWeatherForecast>();
    qRegisterMetaType<KWeatherCore::DailyWeatherForecast>();
    qRegisterMetaType<QList<WeatherLocation *>>();

    // load setup wizard if first launch
    engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
