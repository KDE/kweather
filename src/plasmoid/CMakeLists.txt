#
# SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
#
# SPDX-License-Identifier: GPL-2.0-or-later
#

# TODO: adapt "org.kde.plasma" here & elsewhere if needed (see README)
add_definitions(-DTRANSLATION_DOMAIN=\"plasma_applet_org.kde.plasma.kweather_1x4\")

set(kweather_1x4_SRCS
    kweather_1x4.cpp
    kweather_1x4.h
    hourlymodel.cpp
    hourlymodel.h
)
add_library(plasma_applet_kweather_1x4 MODULE ${kweather_1x4_SRCS})

target_link_libraries(plasma_applet_kweather_1x4
                      Qt::Gui
                      Qt::Qml
                      KWeatherCore
                      Plasma::Plasma
                      KF6::I18n
                      kweatherLib)


install(TARGETS plasma_applet_kweather_1x4 DESTINATION ${KDE_INSTALL_PLUGINDIR}/plasma/applets)

plasma_install_package(package org.kde.plasma.kweather_1x4)
