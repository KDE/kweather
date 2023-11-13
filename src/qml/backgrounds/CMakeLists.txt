qt_add_qml_module(kweatherbackgrounds
    URI "org.kde.kweather.backgrounds"
    VERSION 1.0
    OUTPUT_DIRECTORY "KWeatherBackgrounds"
    QML_FILES
    DynamicBackground.qml
    ClearDay.qml
    ClearNight.qml
    CloudyDay.qml
    CloudyNight.qml
    Misty.qml
    PartlyCloudyDay.qml
    PartlyCloudyNight.qml
    RainyDay.qml
    RainyNight.qml
    SnowyDay.qml
    SnowyNight.qml
    STATIC
)

target_link_libraries(kweather PRIVATE kweatherbackgroundsplugin)

add_subdirectory(components)
