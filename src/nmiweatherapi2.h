#ifndef NMIWEATHERAPI2_H
#define NMIWEATHERAPI2_H

#include <QObject>

#include <KLocalizedContext>
#include <KLocalizedString>
#include <unordered_map>
#include <utility>

#include "abstractweatherapi.h"
#include "abstractweatherforecast.h"

class QXmlStreamReader;
class GeoTimeZone;
// Norwegian Meteorological Institute Weather API Implementation (v2)
// api.met.no

class NMIWeatherAPI2 : public AbstractWeatherAPI
{
Q_OBJECT

public:
    NMIWeatherAPI2();
    ~NMIWeatherAPI2() override;
    void update() override;
    void setLocation(float lat, float lon) override;
    void setToken(QString &) override;
    inline QString &getTimeZone()
    {
        return timeZone;
    };
    inline void setTimeZone(QString tz)
    {
        timeZone = std::move(tz);
    };
private slots:
    void setTZ();
    void parse(QNetworkReply *Reply) override;

private:
    void parseOneElement(QJsonObject& object, QHash<QDate, AbstractDailyWeatherForecast*>& dayCache, QList<AbstractHourlyWeatherForecast*>& hoursList);
    QString timeZone = "Asia/Singapore";
    GeoTimeZone *tz;

    struct ResolvedWeatherDesc {
        QString icon, desc;
        ResolvedWeatherDesc() = default;
        ResolvedWeatherDesc(QString icon, QString desc)
        {
            this->icon = icon;
            this->desc = desc;
        }
    };

    // https://api.met.no/weatherapi/weathericon/2.0/legends
    std::unordered_map<QString, ResolvedWeatherDesc> apiDescMap = {
        {"heavyrainandthunder_neutral", ResolvedWeatherDesc("weather-storm", i18n("Storm"))},
        {"heavyrainandthunder_day", ResolvedWeatherDesc("weather-storm-day", i18n("Storm"))},
        {"heavyrainandthunder_night", ResolvedWeatherDesc("weather-storm-night", i18n("Storm"))},
        {"heavysleetandthunder_neutral", ResolvedWeatherDesc("weather-storm", i18n("Storm"))},
        {"heavysleetandthunder_day", ResolvedWeatherDesc("weather-storm-day", i18n("Storm"))},
        {"heavysleetandthunder_night", ResolvedWeatherDesc("weather-storm-night", i18n("Storm"))},
        {"heavysnowshowersandthunder_neutral", ResolvedWeatherDesc("weather-storm", i18n("Storm"))},
        {"heavysnowshowersandthunder_day", ResolvedWeatherDesc("weather-storm-day", i18n("Storm"))},
        {"heavysnowshowersandthunder_night", ResolvedWeatherDesc("weather-storm-night", i18n("Storm"))},
        {"heavysnow_neutral", ResolvedWeatherDesc("weather-snow", i18n("Heavy Snow"))},
        {"heavysnow_day", ResolvedWeatherDesc("weather-snow", i18n("Heavy Snow"))},
        {"heavysnow_night", ResolvedWeatherDesc("weather-snow", i18n("Heavy Snow"))},
        {"rainandthunder_neutral", ResolvedWeatherDesc("weather-storm", i18n("Storm"))},
        {"rainandthunder_day", ResolvedWeatherDesc("weather-storm-day", i18n("Storm"))},
        {"rainandthunder_night", ResolvedWeatherDesc("weather-storm-night", i18n("Storm"))},
        {"heavysleetshowersandthunder_neutral", ResolvedWeatherDesc("weather-storm", i18n("Storm"))},
        {"heavysleetshowersandthunder_day", ResolvedWeatherDesc("weather-storm-day", i18n("Storm"))},
        {"heavysleetshowersandthunder_night", ResolvedWeatherDesc("weather-storm-night", i18n("Storm"))},
        {"rainshowers_neutral", ResolvedWeatherDesc("weather-showers", i18n("Rain"))},
        {"rainshowers_day", ResolvedWeatherDesc("weather-showers-day", i18n("Rain"))},
        {"rainshowers_night", ResolvedWeatherDesc("weather-showers-night", i18n("Rain"))},
        {"fog_neutral", ResolvedWeatherDesc("weather-fog", i18n("Fog"))},
        {"fog_day", ResolvedWeatherDesc("weather-fog", i18n("Fog"))},
        {"fog_night", ResolvedWeatherDesc("weather-fog", i18n("Fog"))},
        {"heavysleetshowers_neutral", ResolvedWeatherDesc("weather-freezing-rain", i18n("Heavy Sleet"))},
        {"heavysleetshowers_day", ResolvedWeatherDesc("weather-freezing-rain", i18n("Heavy Sleet"))},
        {"heavysleetshowers_night", ResolvedWeatherDesc("weather-freezing-rain", i18n("Heavy Sleet"))},
        {"lightssnowshowersandthunder_neutral", ResolvedWeatherDesc("weather-storm", i18n("Storm"))},
        {"lightssnowshowersandthunder_day", ResolvedWeatherDesc("weather-storm-day", i18n("Storm"))},
        {"lightssnowshowersandthunder_night", ResolvedWeatherDesc("weather-storm-night", i18n("Storm"))},
        {"cloudy_neutral", ResolvedWeatherDesc("weather-clouds", i18n("Cloudy"))},
        {"cloudy_day", ResolvedWeatherDesc("weather-clouds", i18n("Cloudy"))},
        {"cloudy_night", ResolvedWeatherDesc("weather-clouds-night", i18n("Cloudy"))},
        {"snowshowersandthunder_neutral", ResolvedWeatherDesc("weather-storm", i18n("Storm"))},
        {"snowshowersandthunder_day", ResolvedWeatherDesc("weather-storm-day", i18n("Storm"))},
        {"snowshowersandthunder_night", ResolvedWeatherDesc("weather-storm-night", i18n("Storm"))},
        {"lightsnowshowers_neutral", ResolvedWeatherDesc("weather-snow-scattered", i18n("Light Snow"))},
        {"lightsnowshowers_day", ResolvedWeatherDesc("weather-snow-scattered-day", i18n("Light Snow"))},
        {"lightsnowshowers_night", ResolvedWeatherDesc("weather-snow-scattered-night", i18n("Light Snow"))},
        {"heavysleet_neutral", ResolvedWeatherDesc("weather-freezing-rain", i18n("Heavy Sleet"))},
        {"heavysleet_day", ResolvedWeatherDesc("weather-freezing-rain", i18n("Heavy Sleet"))},
        {"heavysleet_night", ResolvedWeatherDesc("weather-freezing-rain", i18n("Heavy Sleet"))},
        {"lightsnowandthunder_neutral", ResolvedWeatherDesc("weather-storm", i18n("Storm"))},
        {"lightsnowandthunder_day", ResolvedWeatherDesc("weather-storm-day", i18n("Storm"))},
        {"lightsnowandthunder_night", ResolvedWeatherDesc("weather-storm-night", i18n("Storm"))},
        {"sleetshowersandthunder_neutral", ResolvedWeatherDesc("weather-storm", i18n("Storm"))},
        {"sleetshowersandthunder_day", ResolvedWeatherDesc("weather-storm-day", i18n("Storm"))},
        {"sleetshowersandthunder_night", ResolvedWeatherDesc("weather-storm-night", i18n("Storm"))},
        {"rainshowersandthunder_neutral", ResolvedWeatherDesc("weather-storm", i18n("Storm"))},
        {"rainshowersandthunder_day", ResolvedWeatherDesc("weather-storm-day", i18n("Storm"))},
        {"rainshowersandthunder_night", ResolvedWeatherDesc("weather-storm-night", i18n("Storm"))},
        {"lightsleet_neutral", ResolvedWeatherDesc("weather-showers-scattered", i18n("Light Sleet"))},
        {"lightsleet_day", ResolvedWeatherDesc("weather-showers-scattered-day", i18n("Light Sleet"))},
        {"lightsleet_night", ResolvedWeatherDesc("weather-showers-scattered-night", i18n("Light Sleet"))},
        {"lightssleetshowersandthunder_neutral", ResolvedWeatherDesc("weather-storm", i18n("Storm"))},
        {"lightssleetshowersandthunder_day", ResolvedWeatherDesc("weather-storm-day", i18n("Storm"))},
        {"lightssleetshowersandthunder_night", ResolvedWeatherDesc("weather-storm-night", i18n("Storm"))},
        {"sleetandthunder_neutral", ResolvedWeatherDesc("weather-storm", i18n("Storm"))},
        {"sleetandthunder_day", ResolvedWeatherDesc("weather-storm-day", i18n("Storm"))},
        {"sleetandthunder_night", ResolvedWeatherDesc("weather-storm-night", i18n("Storm"))},
        {"lightsnow_neutral", ResolvedWeatherDesc("weather-snow-scattered", i18n("Light Snow"))},
        {"lightsnow_day", ResolvedWeatherDesc("weather-snow-scattered-day", i18n("Light Snow"))},
        {"lightsnow_night", ResolvedWeatherDesc("weather-snow-scattered-night", i18n("Light Snow"))},
        {"sleet_neutral", ResolvedWeatherDesc("weather-freezing-rain", i18n("Sleet"))},
        {"sleet_day", ResolvedWeatherDesc("weather-freezing-rain", i18n("Sleet"))},
        {"sleet_night", ResolvedWeatherDesc("weather-freezing-rain", i18n("Sleet"))},
        {"heavyrainshowers_neutral", ResolvedWeatherDesc("weather-showers", i18n("Heavy Rain"))},
        {"heavyrainshowers_day", ResolvedWeatherDesc("weather-showers-day", i18n("Heavy Rain"))},
        {"heavyrainshowers_night", ResolvedWeatherDesc("weather-showers-night", i18n("Heavy Rain"))},
        {"lightsleetshowers_neutral", ResolvedWeatherDesc("weather-showers-scattered", i18n("Light Sleet"))},
        {"lightsleetshowers_day", ResolvedWeatherDesc("weather-showers-scattered-day", i18n("Light Sleet"))},
        {"lightsleetshowers_night", ResolvedWeatherDesc("weather-showers-scattered-night", i18n("Light Sleet"))},
        {"snowshowers_neutral", ResolvedWeatherDesc("weather-snow", i18n("Snow"))},
        {"snowshowers_day", ResolvedWeatherDesc("weather-snow", i18n("Snow"))},
        {"snowshowers_night", ResolvedWeatherDesc("weather-snow", i18n("Snow"))},
        {"snowandthunder_neutral", ResolvedWeatherDesc("weather-snow", i18n("Snow"))},
        {"snowandthunder_day", ResolvedWeatherDesc("weather-snow", i18n("Snow"))},
        {"snowandthunder_night", ResolvedWeatherDesc("weather-snow", i18n("Snow"))},
        {"lightsleetandthunder_neutral", ResolvedWeatherDesc("weather-storm", i18n("Storm"))},
        {"lightsleetandthunder_day", ResolvedWeatherDesc("weather-storm-day", i18n("Storm"))},
        {"lightsleetandthunder_night", ResolvedWeatherDesc("weather-storm-night", i18n("Storm"))},
        {"snow_neutral", ResolvedWeatherDesc("weather-snow", i18n("Snow"))},
        {"snow_day", ResolvedWeatherDesc("weather-snow", i18n("Snow"))},
        {"snow_night", ResolvedWeatherDesc("weather-snow", i18n("Snow"))},
        {"heavyrainshowersandthunder_neutral", ResolvedWeatherDesc("weather-storm", i18n("Storm"))},
        {"heavyrainshowersandthunder_day", ResolvedWeatherDesc("weather-storm-day", i18n("Storm"))},
        {"heavyrainshowersandthunder_night", ResolvedWeatherDesc("weather-storm-night", i18n("Storm"))},
        {"rain_neutral", ResolvedWeatherDesc("weather-showers", i18n("Rain"))},
        {"rain_day", ResolvedWeatherDesc("weather-showers-day", i18n("Rain"))},
        {"rain_night", ResolvedWeatherDesc("weather-showers-night", i18n("Rain"))},
        {"heavysnowshowers_neutral", ResolvedWeatherDesc("weather-snow", i18n("Heavy Snow"))},
        {"heavysnowshowers_day", ResolvedWeatherDesc("weather-snow", i18n("Heavy Snow"))},
        {"heavysnowshowers_night", ResolvedWeatherDesc("weather-snow", i18n("Heavy Snow"))},
        {"fair_neutral", ResolvedWeatherDesc("weather-few-clouds", i18n("Light Clouds"))},
        {"fair_day", ResolvedWeatherDesc("weather-few-clouds", i18n("Partly Sunny"))},
        {"fair_night", ResolvedWeatherDesc("weather-few-clouds-night", i18n("Light Clouds"))},
        {"partlycloudy_neutral", ResolvedWeatherDesc("weather-clouds", i18n("Partly Cloudy"))},
        {"partlycloudy_day", ResolvedWeatherDesc("weather-clouds", i18n("Partly Cloudy"))},
        {"partlycloudy_night", ResolvedWeatherDesc("weather-clouds-night", i18n("Partly Cloudy"))},
        {"clearsky_neutral", ResolvedWeatherDesc("weather-clear", i18n("Clear"))},
        {"clearsky_day", ResolvedWeatherDesc("weather-clear", i18n("Clear"))},
        {"clearsky_night", ResolvedWeatherDesc("weather-clear-night", i18n("Clear"))},
        {"lightrain_neutral", ResolvedWeatherDesc("weather-showers-scattered", i18n("Light Rain"))},
        {"lightrain_day", ResolvedWeatherDesc("weather-showers-scattered-day", i18n("Light Rain"))},
        {"lightrain_night", ResolvedWeatherDesc("weather-showers-scattered-night", i18n("Light Rain"))},
        {"lightrainshowers_neutral", ResolvedWeatherDesc("weather-showers-scattered", i18n("Light Rain"))},
        {"lightrainshowers_day", ResolvedWeatherDesc("weather-showers-scattered-day", i18n("Light Rain"))},
        {"lightrainshowers_night", ResolvedWeatherDesc("weather-showers-scattered-night", i18n("Light Rain"))},
        {"sleetshowers_neutral", ResolvedWeatherDesc("weather-freezing-rain", i18n("Sleet"))},
        {"sleetshowers_day", ResolvedWeatherDesc("weather-freezing-rain", i18n("Sleet"))},
        {"sleetshowers_night", ResolvedWeatherDesc("weather-freezing-rain", i18n("Sleet"))},
        {"lightrainandthunder_neutral", ResolvedWeatherDesc("weather-storm", i18n("Storm"))},
        {"lightrainandthunder_day", ResolvedWeatherDesc("weather-storm-day", i18n("Storm"))},
        {"lightrainandthunder_night", ResolvedWeatherDesc("weather-storm-night", i18n("Storm"))},
        {"lightrainshowersandthunder_neutral", ResolvedWeatherDesc("weather-storm", i18n("Storm"))},
        {"lightrainshowersandthunder_day", ResolvedWeatherDesc("weather-storm-day", i18n("Storm"))},
        {"lightrainshowersandthunder_night", ResolvedWeatherDesc("weather-storm-night", i18n("Storm"))},
        {"heavyrain_neutral", ResolvedWeatherDesc("weather-showers", i18n("Heavy Rain"))},
        {"heavyrain_day", ResolvedWeatherDesc("weather-showers-day", i18n("Heavy Rain"))},
        {"heavyrain_night", ResolvedWeatherDesc("weather-showers-night", i18n("Heavy Rain"))},
    };
};

#endif
