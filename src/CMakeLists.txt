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

qt_add_resources(RESOURCES resources.qrc)

add_executable(kweather ${kweather_SRCS} ${RESOURCES})
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
    KF6::KWeatherCore
    kweatherLib
)

add_subdirectory(qml)
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
        weather-snow
        weather-snow-rain
        weather-snow-scattered
        weather-snow-scattered-day
        weather-snow-scattered-night
        weather-storm
        weather-storm-day
        weather-storm-night
    )
else()
    target_link_libraries(kweather PUBLIC Qt6::Widgets)
endif()

if (UNIX AND (NOT ANDROID))
add_subdirectory(plasmoid)
endif()
target_include_directories(kweather PRIVATE ${CMAKE_BINARY_DIR})
install(TARGETS kweather ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
