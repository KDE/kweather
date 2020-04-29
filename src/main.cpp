#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QMetaObject>
#include <QQmlEngine>
#include <QtQml>
#include <QUrl>

#include <KAboutData>
#include <KConfig>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "weatherlocationmodel.h"

int main(int argc, char *argv[])
{
  QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication app(argc, argv);
  QQmlApplicationEngine engine;

  KLocalizedString::setApplicationDomain("kweather");
  engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
  KAboutData aboutData("kweather", "Weather", "0.1",
                       "Weather application in Kirigami", KAboutLicense::GPL,
                       i18n("© 2020 KDE Community"));
  KAboutData::setApplicationData(aboutData);

  // initialize models in context
  auto *weatherLocationListModel = new WeatherLocationListModel();

  engine.rootContext()->setContextProperty("weatherLocationListModel",
                                           weatherLocationListModel);

  // register QML types
  qmlRegisterType<WeatherLocation>("kweather", 1, 0, "WeatherLocation");
  qmlRegisterType<WeatherDay>(     "kweather", 1, 0, "WeatherDay");
  qmlRegisterType<WeatherHour>(    "kweather", 1, 0, "WeatherHour");

  engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));

  if (engine.rootObjects().isEmpty()) {
    return -1;
  }

  return app.exec();
}
