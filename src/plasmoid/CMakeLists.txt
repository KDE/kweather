#
# SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
# SPDX-FileCopyrightText: 2021 Devin Lin <devin@kde.org>
#
# SPDX-License-Identifier: GPL-2.0-or-later
#

add_definitions(-DTRANSLATION_DOMAIN=\"plasma_applet_org.kde.kweather.plasmoid\")

set(kweatherplasmoid_SRCS
    kweatherplasmoid.cpp
    hourlymodel.cpp
)
add_library(plasma_applet_kweather MODULE ${kweatherplasmoid_SRCS})

kcoreaddons_desktop_to_json(plasma_applet_kweather package/metadata.desktop SERVICE_TYPES plasma-applet.desktop)

target_link_libraries(plasma_applet_kweather
                      Qt5::Gui
                      Qt5::Qml
                      KF5::KWeatherCore
                      KF5::Plasma
                      KF5::I18n
                      kweatherLib)


install(TARGETS plasma_applet_kweather DESTINATION ${KDE_INSTALL_PLUGINDIR}/plasma/applets)

plasma_install_package(package org.kde.kweather.plasmoid)
