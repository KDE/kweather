#
# SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
#
# SPDX-License-Identifier: GPL-2.0-or-later
#

# TODO: adapt "org.kde.plasma" here & elsewhere if needed (see README)
add_definitions(-DTRANSLATION_DOMAIN=\"plasma_applet_org.kde.plasma.kweather_1x4\")

kcoreaddons_add_plugin(org.kde.plasma.kweather_1x4 SOURCES
    kweather_1x4.cpp
    kweather_1x4.h
    hourlymodel.cpp
    hourlymodel.h
    INSTALL_NAMESPACE "plasma/applets"
)

target_link_libraries(org.kde.plasma.kweather_1x4
                      Qt::Gui
                      Qt::Qml
                      KWeatherCore
                      Plasma::Plasma
                      KF6::I18n
                      kweatherLib)


plasma_install_package(package org.kde.plasma.kweather_1x4)
