#ifndef NORWEGIANMETEOROLOGICALINSTITUTE_H
#define NORWEGIANMETEOROLOGICALINSTITUTE_H
#include "abstractapi.h"
#include <QObject>
class QXmlStreamReader;
class NorwegianMeteorologicalInstitute : public AbstractAPI {
  Q_OBJECT

public:

  NorwegianMeteorologicalInstitute();
  void update() override;
  void xmlParse(QXmlStreamReader                      & reader,
                std::vector<AbstractWeatherforecast *>& vector);
  void parseElement(QXmlStreamReader       & reader,
                    AbstractWeatherforecast *fc);

private slots:

  void parse(QNetworkReply *Reply) override;

private:

  // https://api.met.no/weatherapi/weathericon/1.1/documentation
  QHash<int, QString>map = { std::pair<int, QString>(1,  "weather-clear"),
                             std::pair<int, QString>( 2, "weather-few-clouds"),
                             std::pair<int, QString>( 3, "weather-clouds"),
                             std::pair<int, QString>( 4, "weather-many-clouds"),
                             std::pair<int, QString>( 5,
                                                      "weather-showers-scattered"),
                             std::pair<int, QString>( 6, "weather-storm"),
                             std::pair<int, QString>( 7, "weather-snow-rain"),
                             std::pair<int, QString>( 8,
                                                      "weather-snow-scattered-day"),
                             std::pair<int, QString>( 9,
                                                      "weather-showers-scattered"),
                             std::pair<int, QString>(10, "weather-showers"),
                             std::pair<int, QString>(11, "weather-storm"),
                             std::pair<int, QString>(12, "weather-snow-rain"),
                             std::pair<int, QString>(13, "weather-snow"),
                             std::pair<int, QString>(14, "weather-storm"),
                             std::pair<int, QString>(15, "weather-mist"),
                             std::pair<int, QString>(20, "weather-snow-rain"),
                             std::pair<int, QString>(21,
                                                     "weather-snow-scattered-day"),
                             std::pair<int, QString>(22, "weather-storm"),
                             std::pair<int, QString>(23, "weather-storm"),
                             std::pair<int, QString>(24, "weather-storm"),
                             std::pair<int, QString>(25, "weather-storm-day"),
                             std::pair<int, QString>(26, "weather-storm-day"),
                             std::pair<int, QString>(27, "weather-storm-day"),
                             std::pair<int, QString>(28, "weather-storm-day"),
                             std::pair<int, QString>(29, "weather-storm-day"),
                             std::pair<int, QString>(30, "weather-storm"),
                             std::pair<int, QString>(31, "weather-storm"),
                             std::pair<int, QString>(32, "weather-storm"),
                             std::pair<int, QString>(33, "weather-storm"),
                             std::pair<int, QString>(34, "weather-storm"),
                             std::pair<int, QString>(40, "weather-showers-day"),
                             std::pair<int, QString>(41, "weather-showers-day"),
                             std::pair<int, QString>(42, "weather-snow-rain"),
                             std::pair<int, QString>(43, "weather-snow-rain"),
                             std::pair<int, QString>(44,
                                                     "weather-snow-scattered-day"),
                             std::pair<int, QString>(45, "weather-snow-day"),
                             std::pair<int, QString>(46, "weather-showers"),
                             std::pair<int, QString>(47, "weather-snow-rain"),
                             std::pair<int, QString>(48, "weather-snow-rain"),
                             std::pair<int, QString>(49,
                                                     "weather-snow-scattered"),
                             std::pair<int, QString>(50, "weather-snow"),
  };
};

#endif // NORWEGIANMETEOROLOGICALINSTITUTE_H
