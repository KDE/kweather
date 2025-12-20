#
# SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
#
# SPDX-License-Identifier: GPL-2.0-or-later
#

# TODO: adapt "org.kde.plasma" here & elsewhere if needed (see README)
add_definitions(-DTRANSLATION_DOMAIN=\"plasma_applet_org.kde.plasma.kweather.plasmoid.hourly\")

plasma_add_applet(org.kde.plasma.kweather.plasmoid.hourly
    CPP_SOURCES
        kweatherplasmoid.cpp
        kweatherplasmoid.h
        hourlymodel.cpp
        hourlymodel.h
        locationquerymodel.cpp
        locationquerymodel.h
    QML_SOURCES
        qml/main.qml
        qml/WeatherContainer.qml
        qml/WeatherWidget.qml
        qml/config.qml
)

ecm_target_qml_sources(org.kde.plasma.kweather.plasmoid.hourly SOURCES
    qml/settings/ConfigLocation.qml
    PATH settings
)

target_link_libraries(org.kde.plasma.kweather.plasmoid.hourly PRIVATE
                      Qt::Gui
                      Qt::Qml
                      KWeatherCore
                      Plasma::Plasma
                      KF6::I18n
                      kweatherLib)
