#
# SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
# SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
#
# SPDX-License-Identifier: GPL-2.0-or-later
#

set(kweather_SRCS
    main.cpp
    weatherlocationlistmodel.cpp
    weatherlocationlistmodel.h
    weatherlocation.cpp
    weatherlocation.h
    locationquerymodel.cpp
    locationquerymodel.h
    temperaturechartdata.cpp
    temperaturechartdata.h
    formatter.cpp
    formatter.h
)

if (NOT ANDROID)
    set(kweather_SRCS
        ${kweather_SRCS}
        weatherbackground.cpp
        weatherbackground.h
    )
endif()

add_library(kweatherLib STATIC)
set_target_properties(kweatherLib PROPERTIES POSITION_INDEPENDENT_CODE ON)
kconfig_add_kcfg_files(kweatherLib kweathersettings.kcfgc GENERATE_MOC)
target_link_libraries(kweatherLib
    Qt::Core
    KF6::ConfigCore
    KF6::ConfigGui
)

add_executable(kweather ${kweather_SRCS})

qt_add_qml_module(kweather
    URI "org.kde.kweather"
    VERSION 1.0
    QML_FILES
    "qml/Main.qml"
    "qml/FlatLocationForecast.qml"
    "qml/WeatherDayDelegate.qml"
    "qml/WeatherHourDelegate.qml"
    "qml/ForecastContainerPage.qml"
    "qml/DefaultPage.qml"
    "qml/SetupWizard.qml"
    "qml/AboutPage.qml"
    "qml/TemperatureChartCard.qml"
    "qml/SunriseCard.qml"
    "qml/InfoCard.qml"
    "qml/WeatherStrip.qml"
    "qml/DynamicForecastPage.qml"
    "qml/FlatForecastPage.qml"
    SOURCES
    weatherlocationlistmodel.cpp
    weatherlocationlistmodel.h
    DEPENDENCIES
    QtCore
)

qt_add_qml_module(kweatherbackgrounds
    URI "org.kde.kweather.backgrounds"
    VERSION 1.0
    QML_FILES
    "qml/backgrounds/DynamicBackground.qml"
    "qml/backgrounds/ClearDay.qml"
    "qml/backgrounds/ClearNight.qml"
    "qml/backgrounds/CloudyDay.qml"
    "qml/backgrounds/CloudyNight.qml"
    "qml/backgrounds/Misty.qml"
    "qml/backgrounds/PartlyCloudyDay.qml"
    "qml/backgrounds/PartlyCloudyNight.qml"
    "qml/backgrounds/RainyDay.qml"
    "qml/backgrounds/RainyNight.qml"
    "qml/backgrounds/SnowyDay.qml"
    "qml/backgrounds/SnowyNight.qml"
    STATIC
)

qt_add_qml_module(kweatherbackgroundscomponents
    URI "org.kde.kweather.backgrounds.components"
    VERSION 1.0
    QML_FILES
    "qml/backgrounds/components/Sun.qml"
    "qml/backgrounds/components/Stars.qml"
    "qml/backgrounds/components/Snow.qml"
    "qml/backgrounds/components/Rain.qml"
    "qml/backgrounds/components/Cloud.qml"
    "qml/backgrounds/components/Cloudy.qml"
    STATIC
)

qt_add_qml_module(kweathercomponents
    URI "org.kde.kweather.components"
    VERSION 1.0
    QML_FILES
    "qml/components/ListDelegate.qml"
    STATIC
)

qt_add_qml_module(kweatherlocations
    URI "org.kde.kweather.locations"
    VERSION 1.0
    QML_FILES
    "qml/locationslist/LocationsListView.qml"
    "qml/locationslist/LocationsListPage.qml"
    "qml/locationslist/LocationsListDialog.qml"
    "qml/locationslist/AddLocationDialog.qml"
    "qml/locationslist/AddLocationListView.qml"
    "qml/locationslist/AddLocationPage.qml"
    STATIC
)

qt_add_qml_module(kweathersettings
    URI "org.kde.kweather.settings"
    VERSION 1.0
    QML_FILES
    "qml/settings/SettingsPage.qml"
    "qml/settings/SettingsComponent.qml"
    "qml/settings/SettingsDialog.qml"
    "qml/settings/SettingsWindow.qml"
    STATIC
)

target_link_libraries(kweather
    PUBLIC
    Qt6::Qml
    Qt6::QuickControls2
    Qt6::Network
    Qt6::Charts
    Qt6::OpenGL
    KF6::I18n
    KF6::CoreAddons
    KF6::Notifications
    KWeatherCore
    kweatherLib
    kweatherbackgroundsplugin
    kweatherbackgroundscomponentsplugin
    kweathercomponentsplugin
    kweatherlocationsplugin
    kweathersettingsplugin
)

add_subdirectory(resources)

if (ANDROID)
    target_link_libraries(kweather
        PUBLIC
        OpenSSL::SSL
        Qt6::Svg
        Qt6::GuiPrivate
        Qt6::Qml
        Qt6::Quick
    )

    kirigami_package_breeze_icons(ICONS
        raindrop
        globe
        settings-configure
        network-disconnect
        search
        list-add
        delete
        arrow-right
        speedometer
        go-up
        go-down
        go-right
        compass
        mark-location
        find-location

        weather-clear
        weather-clear-night
        weather-clear-wind
        weather-clear-wind-night
        weather-clouds
        weather-clouds-night
        weather-clouds-wind
        weather-clouds-wind-night
        weather-few-clouds
        weather-few-clouds-night
        weather-few-clouds-wind
        weather-few-clouds-wind-night
        weather-fog
        weather-freezing-rain
        weather-freezing-rain-day
        weather-freezing-rain-night
        weather-freezing-scattered-rain
        weather-freezing-scattered-rain-day
        weather-freezing-scattered-rain-night
        weather-freezing-scattered-rain-storm
        weather-freezing-scattered-rain-storm-day
        weather-freezing-scattered-rain-storm-night
        weather-freezing-storm
        weather-freezing-storm-day
        weather-freezing-storm-night
        weather-hail
        weather-many-clouds
        weather-many-clouds-wind
        weather-mist
        weather-none-available
        weather-overcast
        weather-overcast-wind
        weather-showers
        weather-showers-day
        weather-showers-night
        weather-showers-scattered
        weather-showers-scattered-day
        weather-showers-scattered-night
        weather-showers-scattered-storm
        weather-showers-scattered-storm-day
        weather-showers-scattered-storm-night
        weather-snow
        weather-snow-day
        weather-snow-night
        weather-snow-rain
        weather-snow-scattered
        weather-snow-scattered-day
        weather-snow-scattered-night
        weather-snow-scattered-storm
        weather-snow-scattered-storm-day
        weather-snow-scattered-storm-night
        weather-snow-storm
        weather-snow-storm-day
        weather-snow-storm-night
        weather-storm
        weather-storm-day
        weather-storm-night
    )

    ecm_add_android_apk(kweather ANDROID_DIR ${CMAKE_SOURCE_DIR}/android)
else()
    target_link_libraries(kweather PUBLIC Qt6::Widgets)
endif()

if (UNIX AND NOT ANDROID AND BUILD_PLASMOID)
add_subdirectory(plasmoid)
endif()
target_include_directories(kweather PRIVATE ${CMAKE_BINARY_DIR})
install(TARGETS kweather ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
