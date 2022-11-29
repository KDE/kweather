#
# SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
# SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
#
# SPDX-License-Identifier: GPL-2.0-or-later
#

set(kweather_SRCS
    main.cpp
    weatherlocationlistmodel.cpp
    weatherlocation.cpp
    locationquerymodel.cpp
    temperaturechartdata.cpp
    formatter.cpp
)

if (NOT ANDROID)
    set(kweather_SRCS
        ${kweather_SRCS}
        weatherbackground.cpp
    )
endif()

add_library(kweatherLib STATIC)
set_target_properties(kweatherLib PROPERTIES POSITION_INDEPENDENT_CODE ON)
kconfig_add_kcfg_files(kweatherLib kweathersettings.kcfgc GENERATE_MOC)
target_link_libraries(kweatherLib
    Qt::Core
    KF5::ConfigCore
    KF5::ConfigGui
    )
if (QT_MAJOR_VERSION EQUAL "5")
    qtquick_compiler_add_resources(RESOURCES resources.qrc)
else()
    qt_add_resources(RESOURCES resources.qrc)
endif()

add_executable(kweather ${kweather_SRCS} ${RESOURCES})
target_link_libraries(kweather
    PUBLIC
    Qt::Qml
    Qt::QuickControls2
    Qt::Network
    Qt::Charts
    KF5::I18n
    KF5::CoreAddons
    KF5::Notifications
    KF5::KWeatherCore
    kweatherLib
)
if(${QT_MAJOR_VERSION} EQUAL "6")
    target_link_libraries(kweather PRIVATE "Qt${QT_MAJOR_VERSION}::OpenGL")
endif()
if (ANDROID)
    target_link_libraries(kweather
        PUBLIC
        OpenSSL::SSL
        Qt::Svg
        Qt${QT_MAJOR_VERSION}::GuiPrivate
        Qt${QT_MAJOR_VERSION}::Qml
        Qt${QT_MAJOR_VERSION}::Quick
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
    target_link_libraries(kweather PUBLIC Qt::Widgets)
endif()

if (UNIX AND (NOT ANDROID))
# add_subdirectory(plasmoid)
endif()
target_include_directories(kweather PRIVATE ${CMAKE_BINARY_DIR})
install(TARGETS kweather ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
