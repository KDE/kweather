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
    "qml/settings/TemperatureUnitComboBox.qml"
    "qml/settings/SpeedUnitComboBox.qml"
    "qml/settings/PressureUnitComboBox.qml"
    "qml/settings/PrecipitationUnitComboBox.qml"

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
    KF6::I18nQml
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
        arrow-up-symbolic
        arrow-down-symbolic
        arrow-left-symbolic
        arrow-right-symbolic
        compass
        mark-location
        find-location
        system-user-list
        applications-development-symbolic
        checkmark
        temperature-normal-symbolic

        weather-clear
        weather-clear-symbolic
        weather-clear-night
        weather-clear-night-symbolic
        weather-clear-wind
        weather-clear-wind-symbolic
        weather-clear-wind-night
        weather-clear-wind-night-symbolic
        weather-clouds
        weather-clouds-symbolic
        weather-clouds-night
        weather-clouds-night-symbolic
        weather-clouds-wind
        weather-clouds-wind-symbolic
        weather-clouds-wind-night
        weather-clouds-wind-night-symbolic
        weather-few-clouds
        weather-few-clouds-symbolic
        weather-few-clouds-night
        weather-few-clouds-night-symbolic
        weather-few-clouds-wind
        weather-few-clouds-wind-symbolic
        weather-few-clouds-wind-night
        weather-few-clouds-wind-night-symbolic
        weather-fog
        weather-fog-symbolic
        weather-freezing-rain
        weather-freezing-rain-symbolic
        weather-freezing-rain-day
        weather-freezing-rain-day-symbolic
        weather-freezing-rain-night
        weather-freezing-rain-night-symbolic
        weather-freezing-scattered-rain
        weather-freezing-scattered-rain-symbolic
        weather-freezing-scattered-rain-day
        weather-freezing-scattered-rain-day-symbolic
        weather-freezing-scattered-rain-night
        weather-freezing-scattered-rain-night-symbolic
        weather-freezing-scattered-rain-storm
        weather-freezing-scattered-rain-storm-symbolic
        weather-freezing-scattered-rain-storm-day
        weather-freezing-scattered-rain-storm-day-symbolic
        weather-freezing-scattered-rain-storm-night
        weather-freezing-scattered-rain-storm-night-symbolic
        weather-freezing-storm
        weather-freezing-storm-symbolic
        weather-freezing-storm-day
        weather-freezing-storm-day-symbolic
        weather-freezing-storm-night
        weather-freezing-storm-night-symbolic
        weather-hail
        weather-hail-symbolic
        weather-many-clouds
        weather-many-clouds-symbolic
        weather-many-clouds-wind
        weather-many-clouds-wind-symbolic
        weather-mist
        weather-mist-symbolic
        weather-none-available
        weather-none-available-symbolic
        weather-overcast
        weather-overcast-symbolic
        weather-overcast-wind
        weather-overcast-wind-symbolic
        weather-showers
        weather-showers-symbolic
        weather-showers-day
        weather-showers-day-symbolic
        weather-showers-night
        weather-showers-night-symbolic
        weather-showers-scattered
        weather-showers-scattered-symbolic
        weather-showers-scattered-day
        weather-showers-scattered-day-symbolic
        weather-showers-scattered-night
        weather-showers-scattered-night-symbolic
        weather-showers-scattered-storm
        weather-showers-scattered-storm-symbolic
        weather-showers-scattered-storm-day
        weather-showers-scattered-storm-day-symbolic
        weather-showers-scattered-storm-night
        weather-showers-scattered-storm-night-symbolic
        weather-snow
        weather-snow-symbolic
        weather-snow-day
        weather-snow-day-symbolic
        weather-snow-night
        weather-snow-night-symbolic
        weather-snow-rain
        weather-snow-rain-symbolic
        weather-snow-scattered
        weather-snow-scattered-symbolic
        weather-snow-scattered-day
        weather-snow-scattered-day-symbolic
        weather-snow-scattered-night
        weather-snow-scattered-night-symbolic
        weather-snow-scattered-storm
        weather-snow-scattered-storm-symbolic
        weather-snow-scattered-storm-day
        weather-snow-scattered-storm-day-symbolic
        weather-snow-scattered-storm-night
        weather-snow-scattered-storm-night-symbolic
        weather-snow-storm
        weather-snow-storm-symbolic
        weather-snow-storm-day
        weather-snow-storm-day-symbolic
        weather-snow-storm-night
        weather-snow-storm-night-symbolic
        weather-storm
        weather-storm-symbolic
        weather-storm-day
        weather-storm-day-symbolic
        weather-storm-night
        weather-storm-night-symbolic
    )

    ecm_add_android_apk(kweather ANDROID_DIR ${CMAKE_SOURCE_DIR}/android)
else()
    target_link_libraries(kweather PUBLIC Qt6::Widgets KF6::Crash)
endif()

if (UNIX AND NOT ANDROID AND BUILD_PLASMOID)
add_subdirectory(plasmoid)
endif()
target_include_directories(kweather PRIVATE ${CMAKE_BINARY_DIR})
install(TARGETS kweather ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
