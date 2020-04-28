#ifndef NORWEGIANMETEOROLOGICALINSTITUTE_H
#define NORWEGIANMETEOROLOGICALINSTITUTE_H
#include "abstractapi.h"
#include <QObject>
class QXmlStreamReader;
class NorwegianMeteorologicalInstitute :public AbstractAPI
{
    Q_OBJECT
public:
    NorwegianMeteorologicalInstitute();
    void update() override;
    void xmlParse(QXmlStreamReader& reader, std::vector<AbstractWeatherforecast*>& vector);
private slots:
    void parse(QNetworkReply * Reply) override;
};

#endif // NORWEGIANMETEOROLOGICALINSTITUTE_H
