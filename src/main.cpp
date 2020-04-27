#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QUrl>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("KDE");
    QCoreApplication::setOrganizationDomain("kde.org");
    QCoreApplication::setApplicationName("HelloKirigami");

    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:///qml/main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
