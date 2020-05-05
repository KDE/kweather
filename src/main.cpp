#include <QApplication>
#include <QMetaObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QUrl>
#include <QtQml>

#include <KAboutData>
#include <KConfig>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "locationquerymodel.h"
#include "nmiweatherapi.h"
#include "weatherdaymodel.h"
#include "weatherforecastmanager.h"
#include "weatherhourmodel.h"
#include "weatherlocationmodel.h"
AbstractWeatherForecast *tempBuilderUtil(int month, int day, int hour, QString windDirection, QString weatherDesc, QString weatherIcon, int temp, float humidity, float precipitation)
{
    return new AbstractWeatherForecast("Toronto", windDirection, weatherDesc, weatherIcon, weatherIcon, QDateTime(QDate(2020, month, day), QTime(hour, 0)), 32.6532, 79.3832, precipitation, 0, 0, 0.2, temp, temp, humidity, 1000.9);
}
int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    KLocalizedString::setApplicationDomain("kweather");
    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    KAboutData aboutData("kweather", "Weather", "0.1", "Weather application in Kirigami", KAboutLicense::GPL, i18n("© 2020 KDE Community"));
    KAboutData::setApplicationData(aboutData);

    // initialize models in context
    auto *weatherLocationListModel = new WeatherLocationListModel();
    auto *locationQueryModel = new LocationQueryModel();
    WeatherForecastManager::instance(*weatherLocationListModel);

    engine.rootContext()->setContextProperty("weatherLocationListModel", weatherLocationListModel);
    engine.rootContext()->setContextProperty("locationQueryModel", locationQueryModel);
    // the longer the merrier, this add locations
    QObject::connect(locationQueryModel, &LocationQueryModel::appendLocation, [weatherLocationListModel, locationQueryModel] { weatherLocationListModel->addLocation(locationQueryModel->get(locationQueryModel->index_)); });
    // register QML types
    qmlRegisterType<WeatherLocation>("kweather", 1, 0, "WeatherLocation");
    qmlRegisterType<WeatherDay>("kweather", 1, 0, "WeatherDay");
    qmlRegisterType<WeatherHour>("kweather", 1, 0, "WeatherHour");
    qmlRegisterType<AbstractWeatherForecast>("kweather", 1, 0, "AbstractWeatherForecast");
    qmlRegisterType<WeatherHourListModel>("kweather", 1, 0, "WeatherHourListModel");
    qmlRegisterType<WeatherDayListModel>("kweather", 1, 0, "WeatherDayListModel");
    // load example test data for testing purposes TODO
    WeatherLocation *testLocation = new WeatherLocation(new NMIWeatherAPI(), "Toronto", 43.6532, -79.3832);
    WeatherLocation *testLocation2 = new WeatherLocation(new NMIWeatherAPI(), "Singapore", 1.3521, 103.8198);
    QList<AbstractWeatherForecast *> list;
    list.append(tempBuilderUtil(5, 1, 17, "west", "Mostly Cloudy", "weather-clouds", 19, 60.1, 0));
    list.append(tempBuilderUtil(5, 1, 18, "south", "Clear", "weather-clear", 20, 60.1, 0));
    list.append(tempBuilderUtil(5, 1, 19, "west", "Freezing Rain", "weather-freezing-rain", 21, 60.1, 15));
    list.append(tempBuilderUtil(5, 1, 20, "north", "Clear", "weather-clear-night", 24, 60.1, 0));
    list.append(tempBuilderUtil(5, 1, 21, "west", "Mostly Cloudy", "weather-clouds-night", -5, 60.1, 0));
    list.append(tempBuilderUtil(5, 1, 22, "west", "Rain", "weather-showers-night", -6, 60.1, 25));
    list.append(tempBuilderUtil(5, 1, 23, "east", "Rain", "weather-showers-night", -7, 60.1, 26));
    list.append(tempBuilderUtil(5, 2, 0, "west", "Rain", "weather-showers-night", -9, 60.1, 29));
    list.append(tempBuilderUtil(5, 2, 1, "south", "Partly Cloudy", "weather-clouds-night", 15, 60.1, 0));
    list.append(tempBuilderUtil(5, 2, 2, "east", "Partly Cloudy", "weather-clouds-night", 14, 60.1, 0));
    list.append(tempBuilderUtil(5, 2, 3, "west", "Partly Cloudy", "weather-clouds-night", 24, 60.1, 0));
    list.append(tempBuilderUtil(5, 2, 4, "north", "Clear", "weather-clear-night", 25, 60.1, 0));
    list.append(tempBuilderUtil(5, 2, 5, "north", "Clear", "weather-clear-night", 25, 60.1, 0));
    list.append(tempBuilderUtil(5, 2, 6, "west", "Snow", "weather-snow", 30, 60.1, 150));
    list.append(tempBuilderUtil(5, 2, 7, "east", "Snow", "weather-snow-scattered", 15, 60.1, 180));
    list.append(tempBuilderUtil(5, 2, 8, "south", "Thunderstorm", "weather-storm", 14, 60.1, 190));
    list.append(tempBuilderUtil(5, 2, 9, "south", "Thunderstorm", "weather-storm", 13, 60.1, 2400));
    list.append(tempBuilderUtil(5, 2, 10, "west", "Partly Cloudy", "weather-clouds", 5, 60.1, 0));
    list.append(tempBuilderUtil(5, 2, 11, "west", "Clear", "weather-clear", 9, 60.1, 0));
    list.append(tempBuilderUtil(5, 2, 12, "west", "Clear", "weather-clear", 10, 60.1, 0));

    testLocation->updateData(list);
    testLocation2->updateData(list);

    weatherLocationListModel->insert(0, testLocation);
    weatherLocationListModel->insert(1, testLocation2);
    engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
