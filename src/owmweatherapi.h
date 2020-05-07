#ifndef OPENWEATHERMAP_H
#define OPENWEATHERMAP_H
#include "abstractweatherapi.h"
#include <QObject>

// OpenWeatherMap API Implementation
// api.openweathermap.org

class OWMWeatherAPI : public AbstractWeatherAPI
{
    Q_OBJECT

public:
    OWMWeatherAPI(QString locationName);
    ~OWMWeatherAPI();
    void setToken(QString& token) override;
    void setLocation(float latitude, float longitude) override;
    void update() override;

private slots:

    void parse(QNetworkReply* Reply) override;

private:
    QString getWindDirection(double array);
    QHash<QString, QString> map
        = { std::pair<QString, QString>(QStringLiteral("01d"), QStringLiteral("weather-clear")),
            std::pair<QString, QString>(QStringLiteral("01n"), QStringLiteral("weather-clear-night")),
            std::pair<QString, QString>(QStringLiteral("02d"), QStringLiteral("weather-clouds")),
            std::pair<QString, QString>(QStringLiteral("02n"), QStringLiteral("weather-clouds-night")),
            std::pair<QString, QString>(QStringLiteral("03d"), QStringLiteral("weather-many-clouds")),
            std::pair<QString, QString>(QStringLiteral("03n"), QStringLiteral("weather-many-clouds")),
            std::pair<QString, QString>(QStringLiteral("04d"), QStringLiteral("weather-many-clouds")),
            std::pair<QString, QString>(QStringLiteral("04n"), QStringLiteral("weather-many-clouds")),
            std::pair<QString, QString>(QStringLiteral("09d"), QStringLiteral("weather-showers-day")),
            std::pair<QString, QString>(QStringLiteral("09n"), QStringLiteral("weather-showers-night")),
            std::pair<QString, QString>(QStringLiteral("10d"), QStringLiteral("weather-showers-scattered-day")),
            std::pair<QString, QString>(QStringLiteral("10n"), QStringLiteral("weather-showers-scattered-night")),
            std::pair<QString, QString>(QStringLiteral("11d"), QStringLiteral("weather-storm-day")),
            std::pair<QString, QString>(QStringLiteral("11n"), QStringLiteral("weather-storm-night")),
            std::pair<QString, QString>(QStringLiteral("13d"), QStringLiteral("weather-snow-scattered-day")),
            std::pair<QString, QString>(QStringLiteral("13n"), QStringLiteral("weather-storm-scattered-night")),
            std::pair<QString, QString>(QStringLiteral("50d"), QStringLiteral("weather-mist")),
            std::pair<QString, QString>(QStringLiteral("50n"), QStringLiteral("weather-mist")) };
};

#endif // OPENWEATHERMAP_H
