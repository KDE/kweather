#ifndef NORWEGIANMETEOROLOGICALINSTITUTE_H
#define NORWEGIANMETEOROLOGICALINSTITUTE_H

#include <QObject>

#include <KLocalizedContext>
#include <KLocalizedString>
#include <unordered_map>

#include "abstractweatherapi.h"
#include "abstractweatherforecast.h"

class QXmlStreamReader;
class GeoTimeZone;
// Norwegian Meteorological Institute Weather API Implementation
// api.met.no

class NMIWeatherAPI : public AbstractWeatherAPI
{
    Q_OBJECT

public:
    NMIWeatherAPI();
    ~NMIWeatherAPI() override;
    void update() override;
    void setLocation(float lat, float lon) override;
    void setToken(QString &) override;
    inline QString &getTimeZone()
    {
        return timeZone;
    };
    inline void setTimeZone(QString tz)
    {
        timeZone = tz;
    };
private slots:
    void setTZ();
    void parse(QNetworkReply *Reply) override;

private:
    void xmlParse(QXmlStreamReader &reader, QList<AbstractWeatherForecast *> &list);
    void parseElement(QXmlStreamReader &reader, AbstractWeatherForecast *fc);
    QString timeZone = "Asia/Singapore";
    GeoTimeZone *tz;
    // https://api.met.no/weatherapi/weathericon/1.1/documentation

    struct WeatherDescId {
    public:
        int id;
        enum WeatherType {
            NEUTRAL = 0, DAY, NIGHT
        };
        WeatherType type;
        WeatherDescId(int id, WeatherType type) {
            this->id = id;
            this->type = type;
        }
        bool operator== (const WeatherDescId& rhs) const {
            return id == rhs.id && type == rhs.type;
        }
    };
    struct Hasher {
        size_t operator()(const WeatherDescId& r) const {
            return (std::hash<int>()(r.id) * 31) ^ std::hash<int>()(r.type);
        }
    };

    struct ResolvedWeatherDesc {
        QString icon, desc;
        ResolvedWeatherDesc() {}
        ResolvedWeatherDesc(QString icon, QString desc) {
            this->icon = icon;
            this->desc = desc;
        }
    };

    static inline std::pair<WeatherDescId, ResolvedWeatherDesc> getStormNeutral(int id) {
        return {WeatherDescId(id, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-storm", i18n("Storm"))};
    }
    inline std::pair<WeatherDescId, ResolvedWeatherDesc> getStormDay(int id) {
        return {WeatherDescId(id, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-storm-day", i18n("Storm"))};
    }
    inline std::pair<WeatherDescId, ResolvedWeatherDesc> getStormNight(int id) {
        return {WeatherDescId(id, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-storm-night", i18n("Storm"))};
    }

    std::unordered_map<WeatherDescId, ResolvedWeatherDesc, Hasher> apiDescMap = {
        {WeatherDescId(1, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-clear", i18n("Clear"))}, // Sun (neutral)
        {WeatherDescId(1, WeatherDescId::DAY), ResolvedWeatherDesc("weather-clear", i18n("Clear"))}, // Sun (day)
        {WeatherDescId(1, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-clear-night", i18n("Clear"))}, // Sun (night)
        {WeatherDescId(2, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-few-clouds", i18n("Partly Sunny"))}, // LightCloud (neutral)
        {WeatherDescId(2, WeatherDescId::DAY), ResolvedWeatherDesc("weather-few-clouds", i18n("Partly Sunny"))}, // LightCloud (day)
        {WeatherDescId(2, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-few-clouds-night", i18n("Partly Sunny"))}, // LightCloud (night)
        {WeatherDescId(3, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-clouds", i18n("Partly Cloudy"))}, // PartlyCloud (neutral)
        {WeatherDescId(3, WeatherDescId::DAY), ResolvedWeatherDesc("weather-clouds", i18n("Partly Cloudy"))}, // PartlyCloud (day)
        {WeatherDescId(3, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-clouds-night", i18n("Partly Cloudy"))}, // PartlyCloud (night)
        {WeatherDescId(4, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-clouds", i18n("Cloudy"))}, // Cloud (neutral)
        {WeatherDescId(4, WeatherDescId::DAY), ResolvedWeatherDesc("weather-clouds", i18n("Cloudy"))}, // Cloud (day)
        {WeatherDescId(4, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-clouds-night", i18n("Cloudy"))}, // Cloud (night)
        {WeatherDescId(5, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-showers-scattered", i18n("Light Rain"))}, // LightRainSun (neutral)
        {WeatherDescId(5, WeatherDescId::DAY), ResolvedWeatherDesc("weather-showers-scattered-day", i18n("Light Rain"))}, // LightRainSun (day)
        {WeatherDescId(5, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-showers-scattered-night", i18n("Light Rain"))}, // LightRainSun (night)
        {WeatherDescId(6, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-storm", i18n("Storm"))}, // LightRainThunderSun (neutral)
        {WeatherDescId(6, WeatherDescId::DAY), ResolvedWeatherDesc("weather-storm-day", i18n("Storm"))}, // LightRainThunderSun (day)
        {WeatherDescId(6, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-storm-night", i18n("Storm"))}, // LightRainThunderSun (night)
        {WeatherDescId(7, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-freezing-rain", i18n("Sleet"))}, // SleetSun (neutral)
        {WeatherDescId(7, WeatherDescId::DAY), ResolvedWeatherDesc("weather-freezing-rain", i18n("Sleet"))}, // SleetSun (day)
        {WeatherDescId(7, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-freezing-rain", i18n("Sleet"))}, // SleetSun (night)
        {WeatherDescId(8, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-snow", i18n("Snow"))}, // SnowSun (neutral)
        {WeatherDescId(8, WeatherDescId::DAY), ResolvedWeatherDesc("weather-snow", i18n("Snow"))}, // SnowSun (day)
        {WeatherDescId(8, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-snow", i18n("Snow"))}, // SnowSun (night)
        {WeatherDescId(9, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-showers-scattered", i18n("Light Rain"))}, // LightRain (neutral)
        {WeatherDescId(9, WeatherDescId::DAY), ResolvedWeatherDesc("weather-showers-scattered-day", i18n("Light Rain"))}, // LightRain (day)
        {WeatherDescId(9, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-showers-scattered-night", i18n("Light Rain"))}, // LightRain (night)
        {WeatherDescId(10, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-showers", i18n("Rain"))}, // Rain (neutral)
        {WeatherDescId(10, WeatherDescId::DAY), ResolvedWeatherDesc("weather-showers-day", i18n("Rain"))}, // Rain (day)
        {WeatherDescId(10, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-showers-night", i18n("Rain"))}, // Rain (night)
        {WeatherDescId(11, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-storm", i18n("Storm"))}, // RainThunder (neutral)
        {WeatherDescId(11, WeatherDescId::DAY), ResolvedWeatherDesc("weather-storm-day", i18n("Storm"))}, // RainThunder (day)
        {WeatherDescId(11, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-storm-night", i18n("Storm"))}, // RainThunder (night)
        {WeatherDescId(12, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-freezing-rain", i18n("Sleet"))}, // Sleet (neutral)
        {WeatherDescId(12, WeatherDescId::DAY), ResolvedWeatherDesc("weather-freezing-rain", i18n("Sleet"))}, // Sleet (day)
        {WeatherDescId(12, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-freezing-rain", i18n("Sleet"))}, // Sleet (night)
        {WeatherDescId(13, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-snow", i18n("Snow"))}, // Snow (neutral)
        {WeatherDescId(13, WeatherDescId::DAY), ResolvedWeatherDesc("weather-snow", i18n("Snow"))}, // Snow (day)
        {WeatherDescId(13, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-snow", i18n("Snow"))}, // Snow (night)
        {WeatherDescId(14, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-storm", i18n("Snow"))}, // SnowThunder (neutral)
        {WeatherDescId(14, WeatherDescId::DAY), ResolvedWeatherDesc("weather-storm-day", i18n("Snow"))}, // SnowThunder (day)
        {WeatherDescId(14, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-storm-night", i18n("Snow"))}, // SnowThunder (night)
        {WeatherDescId(15, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-fog", i18n("Fog"))}, // Fog (neutral)
        {WeatherDescId(15, WeatherDescId::DAY), ResolvedWeatherDesc("weather-fog", i18n("Fog"))}, // Fog (day)
        {WeatherDescId(15, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-fog", i18n("Fog"))}, // Fog (night)
        getStormNeutral(20), // SleetSunThunder (neutral)
        getStormDay(20), // SleetSunThunder (day)
        getStormNight(20), // SleetSunThunder (night)
        getStormNeutral(21), // SnowSunThunder (neutral)
        getStormDay(21), // SnowSunThunder (day)
        getStormNight(21), // SnowSunThunder (night)
        getStormNeutral(22), // LightRainThunder (neutral)
        getStormDay(22), // LightRainThunder (day)
        getStormNight(22), // LightRainThunder (night)
        getStormNeutral(23), // SleetThunder (neutral)
        getStormDay(23), // SleetThunder (day)
        getStormNight(23), // SleetThunder (night)
        getStormNeutral(24), // DrizzleThunderSun (neutral)
        getStormDay(24), // DrizzleThunderSun (day)
        getStormNight(24), // DrizzleThunderSun (night)
        getStormNeutral(25), // RainThunderSun (neutral)
        getStormDay(25), // RainThunderSun (day)
        getStormNight(25), // RainThundeSun (night)
        getStormNeutral(26), // LightSleetThunderSun (neutral)
        getStormDay(26), // LightSleetThunderSun (day)
        getStormNight(26), // LightSleetThunderSun (night)
        getStormNeutral(27), // HeavySleetThunderSun (neutral)
        getStormDay(27), // HeavySleetThunderSun (day)
        getStormNight(27), // HeavySleetThunderSun (night)
        getStormNeutral(28), // LightSnowThunderSun (neutral)
        getStormDay(28), // LightSnowThunderSun (day)
        getStormNight(28), // LightSnowThunderSun (night)
        getStormNeutral(29), // HeavySnowThunderSun (neutral)
        getStormDay(29), // HeavySnowThunderSun (day)
        getStormNight(29), // HeavySnowThunderSun (night)
        getStormNeutral(30), // DrizzleThunder (neutral)
        getStormDay(30), // DrizzleThunder (day)
        getStormNight(30), // DrizzleThunder (night)
        getStormNeutral(31), // LightSleetThunder (neutral)
        getStormDay(31), // LightSleetThunder (day)
        getStormNight(31), // LightSleetThunder (night)
        getStormNeutral(32), // HeavySleetThunder (neutral)
        getStormDay(32), // HeavySleetThunder (day)
        getStormNight(32), // HeavySleetThunder (night)
        getStormNeutral(33), // LightSnowThunder (neutral)
        getStormDay(33), // LightSnowThunder (day)
        getStormNight(33), // LightSnowThunder (night)
        getStormNeutral(34), // HeavySnowThunder (neutral)
        getStormDay(34), // HeavySnowThunder (day)
        getStormNight(34), // HeavySnowThunder (night)
        {WeatherDescId(40, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-mist", i18n("Drizzle"))}, // DrizzleSun (neutral)
        {WeatherDescId(40, WeatherDescId::DAY), ResolvedWeatherDesc("weather-mist", i18n("Drizzle"))}, // DrizzleSun (day)
        {WeatherDescId(40, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-mist", i18n("Drizzle"))}, // DrizzleSun (night)
        {WeatherDescId(41, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-showers", i18n("Rain"))}, // RainSun (neutral)
        {WeatherDescId(41, WeatherDescId::DAY), ResolvedWeatherDesc("weather-showers-day", i18n("Rain"))}, // RainSun (day)
        {WeatherDescId(41, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-showers-night", i18n("Rain"))}, // RainSun (night)
        {WeatherDescId(42, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-showers-scattered", i18n("Sleet"))}, // LightSleetSun (neutral)
        {WeatherDescId(42, WeatherDescId::DAY), ResolvedWeatherDesc("weather-showers-scattered-day", i18n("Sleet"))}, // LightSleetSun (day)
        {WeatherDescId(42, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-showers-scattered-night", i18n("Sleet"))}, // LightSleetSun (night)
        {WeatherDescId(43, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-freezing-rain", i18n("Heavy Sleet"))}, // HeavySleetSun (neutral)
        {WeatherDescId(43, WeatherDescId::DAY), ResolvedWeatherDesc("weather-freezing-rain", i18n("Heavy Sleet"))}, // HeavySleetSun (day)
        {WeatherDescId(43, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-freezing-rain", i18n("Heavy Sleet"))}, // HeavySleetSun (night)
        {WeatherDescId(44, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-snow-scattered", i18n("Light Snow"))}, // LightSnowSun (neutral)
        {WeatherDescId(44, WeatherDescId::DAY), ResolvedWeatherDesc("weather-snow-scattered-day", i18n("Light Snow"))}, // LightSnowSun (day)
        {WeatherDescId(44, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-snow-scattered-night", i18n("Light Snow"))}, // LightSnowSun (night)
        {WeatherDescId(45, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-snow", i18n("Heavy Snow"))}, // HeavySnowSun (neutral)
        {WeatherDescId(45, WeatherDescId::DAY), ResolvedWeatherDesc("weather-snow", i18n("Heavy Snow"))}, // HeavySnowSun (day)
        {WeatherDescId(45, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-snow", i18n("Heavy Snow"))}, // HeavySnowSun (night)
        {WeatherDescId(46, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-mist", i18n("Drizzle"))}, // Drizzle (neutral)
        {WeatherDescId(46, WeatherDescId::DAY), ResolvedWeatherDesc("weather-mist", i18n("Drizzle"))}, // Drizzle (day)
        {WeatherDescId(46, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-mist", i18n("Drizzle"))}, // Drizzle (night)
        {WeatherDescId(47, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-showers-scattered", i18n("Sleet"))}, // LightSleet (neutral)
        {WeatherDescId(47, WeatherDescId::DAY), ResolvedWeatherDesc("weather-showers-scattered-day", i18n("Sleet"))}, // LightSleet (day)
        {WeatherDescId(47, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-showers-scattered-night", i18n("Sleet"))}, // LightSleet (night)
        {WeatherDescId(48, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-freezing-rain", i18n("Heavy Sleet"))}, // HeavySleet (neutral)
        {WeatherDescId(48, WeatherDescId::DAY), ResolvedWeatherDesc("weather-freezing-rain", i18n("Heavy Sleet"))}, // HeavySleet (day)
        {WeatherDescId(48, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-freezing-rain", i18n("Heavy Sleet"))}, // HeavySleet (night)
        {WeatherDescId(49, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-snow-scattered", i18n("Light Snow"))}, // LightSnow (neutral)
        {WeatherDescId(49, WeatherDescId::DAY), ResolvedWeatherDesc("weather-snow-scattered-day", i18n("Light Snow"))}, // LightSnow (day)
        {WeatherDescId(49, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-snow-scattered-night", i18n("Light Snow"))}, // LightSnow (night)
        {WeatherDescId(50, WeatherDescId::NEUTRAL), ResolvedWeatherDesc("weather-snow", i18n("Heavy Snow"))}, // HeavySnow (neutral)
        {WeatherDescId(50, WeatherDescId::DAY), ResolvedWeatherDesc("weather-snow", i18n("Heavy Snow"))}, // HeavySnow (day)
        {WeatherDescId(50, WeatherDescId::NIGHT), ResolvedWeatherDesc("weather-snow", i18n("Heavy Snow"))}, // HeavySnow (night)

    };

//    QHash<WeatherDescriptionId, ResolvedWeatherDescription> map = {
//        std::make_pair(WeatherDescriptionId(1, true), QStringLiteral("weather-clear")),
//        std::pair<int, QString>(2, QStringLiteral("weather-few-clouds")),
//        std::pair<int, QString>(3, QStringLiteral("weather-clouds")),
//        std::pair<int, QString>(4, QStringLiteral("weather-many-clouds")),
//        std::pair<int, QString>(5, QStringLiteral("weather-showers-scattered")),
//        std::pair<int, QString>(6, QStringLiteral("weather-storm")),
//        std::pair<int, QString>(7, QStringLiteral("weather-snow-rain")),
//        std::pair<int, QString>(8, QStringLiteral("weather-snow-scattered-day")),
//        std::pair<int, QString>(9, QStringLiteral("weather-showers-scattered")),
//        std::pair<int, QString>(10, QStringLiteral("weather-showers")),
//        std::pair<int, QString>(11, QStringLiteral("weather-storm")),
//        std::pair<int, QString>(12, QStringLiteral("weather-snow-rain")),
//        std::pair<int, QString>(13, QStringLiteral("weather-snow")),
//        std::pair<int, QString>(14, QStringLiteral("weather-storm")),
//        std::pair<int, QString>(15, QStringLiteral("weather-mist")),
//        std::pair<int, QString>(20, QStringLiteral("weather-snow-rain")),
//        std::pair<int, QString>(21, QStringLiteral("weather-snow-scattered-day")),
//        std::pair<int, QString>(22, QStringLiteral("weather-storm")),
//        std::pair<int, QString>(23, QStringLiteral("weather-storm")),
//        std::pair<int, QString>(24, QStringLiteral("weather-storm")),
//        std::pair<int, QString>(25, QStringLiteral("weather-storm-day")),
//        std::pair<int, QString>(26, QStringLiteral("weather-storm-day")),
//        std::pair<int, QString>(27, QStringLiteral("weather-storm-day")),
//        std::pair<int, QString>(28, QStringLiteral("weather-storm-day")),
//        std::pair<int, QString>(29, QStringLiteral("weather-storm-day")),
//        std::pair<int, QString>(30, QStringLiteral("weather-storm")),
//        std::pair<int, QString>(31, QStringLiteral("weather-storm")),
//        std::pair<int, QString>(32, QStringLiteral("weather-storm")),
//        std::pair<int, QString>(33, QStringLiteral("weather-storm")),
//        std::pair<int, QString>(34, QStringLiteral("weather-storm")),
//        std::pair<int, QString>(40, QStringLiteral("weather-showers-day")),
//        std::pair<int, QString>(41, QStringLiteral("weather-showers-day")),
//        std::pair<int, QString>(42, QStringLiteral("weather-snow-rain")),
//        std::pair<int, QString>(43, QStringLiteral("weather-snow-rain")),
//        std::pair<int, QString>(44, QStringLiteral("weather-snow-scattered-day")),
//        std::pair<int, QString>(45, QStringLiteral("weather-snow-day")),
//        std::pair<int, QString>(46, QStringLiteral("weather-showers")),
//        std::pair<int, QString>(47, QStringLiteral("weather-snow-rain")),
//        std::pair<int, QString>(48, QStringLiteral("weather-snow-rain")),
//        std::pair<int, QString>(49, QStringLiteral("weather-snow-scattered")),
//        std::pair<int, QString>(50, QStringLiteral("weather-snow")),
//    };
};

#endif // NORWEGIANMETEOROLOGICALINSTITUTE_H
