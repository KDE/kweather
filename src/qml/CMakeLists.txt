qt_add_qml_module(kweather
    URI "org.kde.kweather"
    VERSION 1.0
    OUTPUT_DIRECTORY "KWeather"
    # RESOURCE_PREFIX "/"
    QML_FILES
    Main.qml
    FlatLocationForecast.qml
    WeatherDayDelegate.qml
    WeatherHourDelegate.qml
    ForecastContainerPage.qml
    DefaultPage.qml
    SetupWizard.qml
    AboutPage.qml
    TemperatureChartCard.qml
    SunriseCard.qml
    InfoCard.qml
    WeatherStrip.qml
    DynamicForecastPage.qml
    FlatForecastPage.qml
)

add_subdirectory(settings)
add_subdirectory(locationslist)
add_subdirectory(components)
add_subdirectory(backgrounds)
