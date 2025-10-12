#
# SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
#
# SPDX-License-Identifier: GPL-2.0-or-later
#

# TODO: adapt "org.kde.plasma" here & elsewhere if needed (see README)
add_definitions(-DTRANSLATION_DOMAIN=\"plasma_applet_org.kde.plasma.kweather_1x4\")

plasma_add_applet(org.kde.plasma.kweather_1x4
    CPP_SOURCES
        kweather_1x4.cpp
        kweather_1x4.h
        hourlymodel.cpp
        hourlymodel.h
    QML_SOURCES
        main.qml
        LocationSelector.qml
        WeatherContainer.qml
)

target_link_libraries(org.kde.plasma.kweather_1x4 PRIVATE
                      Qt::Gui
                      Qt::Qml
                      KWeatherCore
                      Plasma::Plasma
                      KF6::I18n
                      kweatherLib)
