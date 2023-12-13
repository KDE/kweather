/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts

import org.kde.kirigami as Kirigami
import org.kde.kholidays
import org.kde.kweather

import org.kde.kweather.backgrounds


Kirigami.Card {
    id: root

    required property var selectedDay
    property var textColor: Kirigami.Theme.textColor

    contentItem: Item {
        implicitHeight: sunsetColumn.height

        Column {
            id: sunsetColumn
            spacing: Kirigami.Units.largeSpacing * 2

            // Sunrise
            RowLayout {
                spacing: Kirigami.Units.largeSpacing
                Kirigami.Icon {
                    source: "go-up"
                    Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                    Layout.minimumWidth: Layout.minimumHeight * 1.5
                    isMask: true
                    color: settingsModel && settingsModel.forecastStyle === "Dynamic" ? weatherLocation.iconColor : Kirigami.Theme.textColor
                }
                Column {
                    spacing: Kirigami.Units.smallSpacing
                    Label {
                        font.weight: Font.Bold
                        text: i18n("Sunrise")
                        color: root.textColor
                    }
                    Label {
                        text: Formatter.formatSunriseTime(SunRiseSet.utcSunrise(selectedDay.date, weatherLocation.latitude, weatherLocation.longitude), weatherLocation.timeZone)
                        color: root.textColor
                    }
                }
            }

            // Sunset
            RowLayout {
                spacing: Kirigami.Units.largeSpacing
                Kirigami.Icon {
                    source: "go-down"
                    Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                    Layout.minimumWidth: Layout.minimumHeight * 1.5
                    isMask: true
                    color: settingsModel && settingsModel.forecastStyle === "Dynamic" ? weatherLocation.iconColor : Kirigami.Theme.textColor
                }
                Column {
                    spacing: Kirigami.Units.smallSpacing
                    Label {
                        font.weight: Font.Bold
                        text: i18n("Sunset")
                        color: root.textColor
                    }
                    Label {
                        text: Formatter.formatSunriseTime(SunRiseSet.utcSunset(selectedDay.date, weatherLocation.latitude, weatherLocation.longitude), weatherLocation.timeZone)
                        color: root.textColor
                    }
                }
            }

            // Moon phase
            RowLayout {
                spacing: Kirigami.Units.largeSpacing
                Kirigami.Icon {
                    source: "weather-clear-night"
                    Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                    Layout.minimumWidth: Layout.minimumHeight * 1.5
                }
                Column {
                    spacing: Kirigami.Units.smallSpacing
                    Label {
                        font.weight: Font.Bold
                        text: i18n("Moon Phase")
                        color: root.textColor
                    }
                    Label {
                        text: Lunar.phaseNameAtDate(selectedDay.date)
                        color: root.textColor
                    }
                }
            }
        }
    }
}
