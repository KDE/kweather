qt_add_qml_module(kweatherbackgroundscomponents
    URI "org.kde.kweather.backgrounds.components"
    VERSION 1.0
    OUTPUT_DIRECTORY "KWeatherBackgroundsComponents"
    QML_FILES
    Sun.qml
    Stars.qml
    Snow.qml
    Rain.qml
    Cloud.qml
    Cloudy.qml
    STATIC
)

target_link_libraries(kweather PRIVATE kweatherbackgroundscomponentsplugin)
