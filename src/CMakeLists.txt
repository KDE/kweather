#
# Copyright 2020 Han Young <hanyoung@protonmail.com>
# Copyright 2020 Devin Lin <espidev@gmail.com>
#
# SPDX-License-Identifier: GPL-2.0-or-later
#

set(kweather_SRCS
    main.cpp
    geoiplookup.cpp
    geolocation.cpp
    abstractweatherforecast.cpp
    weatherforecastmanager.cpp
    weatherlocationmodel.cpp
    weatherhourmodel.cpp
    weatherdaymodel.cpp
    owmweatherapi.cpp
    abstractweatherapi.cpp
    geotimezone.cpp
    locationquerymodel.cpp
    abstractdailyweatherforecast.cpp
    abstracthourlyweatherforecast.cpp
    abstracthourlyweatherforecast.h
    nmiweatherapi2.cpp
    nmiweatherapi2.h
        settingsmodel.cpp settingsmodel.h
    sunriseset.cpp
    abstractsunrise.cpp
        )

qt5_add_resources(RESOURCES resources.qrc)
add_executable(kweather ${kweather_SRCS} ${RESOURCES})
target_link_libraries(kweather
        Qt5::Core
        Qt5::Qml
        Qt5::QuickControls2
        Qt5::Widgets
        Qt5::Network
        KF5::I18n
        KF5::ConfigCore
        KF5::CoreAddons
        KF5::Notifications
        )

install(TARGETS kweather ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})
