#ifndef NORWEGIANMETEOROLOGICALINSTITUTE_H
#define NORWEGIANMETEOROLOGICALINSTITUTE_H

#include <QObject>

#include "abstractweatherforecast.h"
#include "abstractapi.h"

class QXmlStreamReader;
class NorwegianMeteorologicalInstitute : public AbstractAPI {
  Q_OBJECT

public:

  NorwegianMeteorologicalInstitute();
  void update() override;
  void setLocation(float lat,
                   float lon) override;
  void setToken(QString&) override;

private slots:

  void parse(QNetworkReply *Reply) override;

private:

  void xmlParse(QXmlStreamReader                      & reader,
                std::vector<AbstractWeatherForecast *>& vector);
  bool parseElement(QXmlStreamReader       & reader,
                    AbstractWeatherForecast *fc);

  // https://api.met.no/weatherapi/weathericon/1.1/documentation
  // it's qtcreator auto indentation, I didn't write this!!!
  QHash<int, QString>map = { std::pair<int,
                                       QString>(1,
                                                QStringLiteral("weather-clear")),
                             std::pair<int,
                                       QString>(2,
                                                QStringLiteral(
                                                  "weather-few-clouds")),
                             std::pair<int,
                                       QString>(3,
                                                QStringLiteral("weather-clouds")),
                             std::pair<int,
                                       QString>(4,
                                                QStringLiteral(
                                                  "weather-many-clouds")),
                             std::pair<int,
                                       QString>(5,
                                                QStringLiteral(
                                                  "weather-showers-scattered")),
                             std::pair<int,
                                       QString>(6,
                                                QStringLiteral("weather-storm")),
                             std::pair<int,
                                       QString>(7,
                                                QStringLiteral(
                                                  "weather-snow-rain")),
                             std::pair<int,
                                       QString>(8,
                                                QStringLiteral(
                                                  "weather-snow-scattered-day")),
                             std::pair<int,
                                       QString>(9,
                                                QStringLiteral(
                                                  "weather-showers-scattered")),
                             std::pair<int,
                                       QString>(10,
                                                QStringLiteral("weather-showers")),
                             std::pair<int,
                                       QString>(11,
                                                QStringLiteral("weather-storm")),
                             std::pair<int,
                                       QString>(12,
                                                QStringLiteral(
                                                  "weather-snow-rain")),
                             std::pair<int,
                                       QString>(13,
                                                QStringLiteral("weather-snow")),
                             std::pair<int,
                                       QString>(14,
                                                QStringLiteral("weather-storm")),
                             std::pair<int,
                                       QString>(15,
                                                QStringLiteral("weather-mist")),
                             std::pair<int,
                                       QString>(20,
                                                QStringLiteral(
                                                  "weather-snow-rain")),
                             std::pair<int,
                                       QString>(21,
                                                QStringLiteral(
                                                  "weather-snow-scattered-day")),
                             std::pair<int,
                                       QString>(22,
                                                QStringLiteral("weather-storm")),
                             std::pair<int,
                                       QString>(23,
                                                QStringLiteral("weather-storm")),
                             std::pair<int,
                                       QString>(24,
                                                QStringLiteral("weather-storm")),
                             std::pair<int,
                                       QString>(25,
                                                QStringLiteral(
                                                  "weather-storm-day")),
                             std::pair<int,
                                       QString>(26,
                                                QStringLiteral(
                                                  "weather-storm-day")),
                             std::pair<int,
                                       QString>(27,
                                                QStringLiteral(
                                                  "weather-storm-day")),
                             std::pair<int,
                                       QString>(28,
                                                QStringLiteral(
                                                  "weather-storm-day")),
                             std::pair<int,
                                       QString>(29,
                                                QStringLiteral(
                                                  "weather-storm-day")),
                             std::pair<int,
                                       QString>(30,
                                                QStringLiteral("weather-storm")),
                             std::pair<int,
                                       QString>(31,
                                                QStringLiteral("weather-storm")),
                             std::pair<int,
                                       QString>(32,
                                                QStringLiteral("weather-storm")),
                             std::pair<int,
                                       QString>(33,
                                                QStringLiteral("weather-storm")),
                             std::pair<int,
                                       QString>(34,
                                                QStringLiteral("weather-storm")),
                             std::pair<int,
                                       QString>(40,
                                                QStringLiteral(
                                                  "weather-showers-day")),
                             std::pair<int,
                                       QString>(41,
                                                QStringLiteral(
                                                  "weather-showers-day")),
                             std::pair<int,
                                       QString>(42,
                                                QStringLiteral(
                                                  "weather-snow-rain")),
                             std::pair<int,
                                       QString>(43,
                                                QStringLiteral(
                                                  "weather-snow-rain")),
                             std::pair<int,
                                       QString>(44,
                                                QStringLiteral(
                                                  "weather-snow-scattered-day")),
                             std::pair<int,
                                       QString>(45,
                                                QStringLiteral("weather-snow-day")),
                             std::pair<int,
                                       QString>(46,
                                                QStringLiteral("weather-showers")),
                             std::pair<int,
                                       QString>(47,
                                                QStringLiteral(
                                                  "weather-snow-rain")),
                             std::pair<int,
                                       QString>(48,
                                                QStringLiteral(
                                                  "weather-snow-rain")),
                             std::pair<int,
                                       QString>(49,
                                                QStringLiteral(
                                                  "weather-snow-scattered")),
                             std::pair<int,
                                       QString>(50,
                                                QStringLiteral("weather-snow")),
  };
};

#endif // NORWEGIANMETEOROLOGICALINSTITUTE_H
