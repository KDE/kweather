qt_add_qml_module(kweatherlocations
    URI "org.kde.kweather.locations"
    VERSION 1.0
    OUTPUT_DIRECTORY "KWeatherLocations"
    QML_FILES
    "LocationsListView.qml"
    "LocationsListPage.qml"
    "LocationsListDialog.qml"
    "AddLocationDialog.qml"
    "AddLocationListView.qml"
    "AddLocationPage.qml"
    STATIC
)

target_link_libraries(kweather PRIVATE kweatherlocationsplugin)
