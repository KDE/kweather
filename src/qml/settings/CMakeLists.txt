qt_add_qml_module(kweathersettings
    URI "org.kde.kweather.settings"
    VERSION 1.0
    OUTPUT_DIRECTORY "KWeatherSettings"
    QML_FILES
    "SettingsPage.qml"
    "SettingsComponent.qml"
    "SettingsDialog.qml"
    "SettingsWindow.qml"
    STATIC
)

target_link_libraries(kweather PRIVATE kweathersettingsplugin)
