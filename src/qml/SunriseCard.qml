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

Kirigami.AbstractCard {
    id: root
    topPadding: Kirigami.Units.gridUnit
    bottomPadding: Kirigami.Units.gridUnit
    leftPadding: Kirigami.Units.gridUnit
    rightPadding: Kirigami.Units.gridUnit

    required property var selectedDay
    property var textColor: Kirigami.Theme.textColor

    contentItem: Item {
        implicitHeight: sunsetColumn.height

        Column {
            id: sunsetColumn
            spacing: Kirigami.Units.largeSpacing * 2

            // Sunrise
            RowLayout {
                spacing: Kirigami.Units.gridUnit
                Kirigami.Icon {
                    source: "go-up"
                    implicitHeight: Kirigami.Units.iconSizes.medium
                    implicitWidth: Kirigami.Units.iconSizes.medium
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
                spacing: Kirigami.Units.gridUnit
                Kirigami.Icon {
                    source: "go-down"
                    implicitHeight: Kirigami.Units.iconSizes.medium
                    implicitWidth: Kirigami.Units.iconSizes.medium
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
                spacing: Kirigami.Units.gridUnit
                Kirigami.Icon {
                    source: "weather-clear-night"
                    implicitHeight: Kirigami.Units.iconSizes.medium
                    implicitWidth: Kirigami.Units.iconSizes.medium
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
