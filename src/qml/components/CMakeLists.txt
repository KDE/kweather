qt_add_qml_module(kweathercomponents
    URI "org.kde.kweather.components"
    VERSION 1.0
    OUTPUT_DIRECTORY "KWeatherComponents"
    QML_FILES
    ListDelegate.qml
    STATIC
)

target_link_libraries(kweather PRIVATE kweathercomponentsplugin)
