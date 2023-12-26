/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts

import org.kde.kirigami as Kirigami
import org.kde.kweather
import org.kde.kweather.backgrounds

Kirigami.AbstractCard {
    id: root
    topPadding: Kirigami.Units.gridUnit
    bottomPadding: Kirigami.Units.gridUnit
    leftPadding: Kirigami.Units.gridUnit
    rightPadding: Kirigami.Units.gridUnit

    property var textColor: Kirigami.Theme.textColor

    contentItem: Item {
        implicitHeight: column.height

        Column {
            id: column
            spacing: Kirigami.Units.largeSpacing * 2

            // precipitation
            RowLayout {
                spacing: Kirigami.Units.gridUnit
                Kirigami.Icon {
                    source: "raindrop"
                    implicitHeight: Kirigami.Units.iconSizes.medium
                    implicitWidth: Kirigami.Units.iconSizes.medium
                    color: settingsModel && settingsModel.forecastStyle === "Dynamic" ? weatherLocation.iconColor : Kirigami.Theme.textColor
                    isMask: true
                }
                Column {
                    spacing: Kirigami.Units.smallSpacing
                    Label {
                        font.weight: Font.Bold
                        text: i18n("Precipitation")
                        color: root.textColor
                    }
                    Label {
                        text: selectedDay == null ? "" : Formatter.formatPrecipitation(selectedDay.precipitation, settingsModel.precipitationUnits)
                        color: root.textColor
                    }
                }
            }

            // Humidity
            RowLayout {
                spacing: Kirigami.Units.gridUnit
                Kirigami.Icon {
                    source: "compass"
                    implicitHeight: Kirigami.Units.iconSizes.medium
                    implicitWidth: Kirigami.Units.iconSizes.medium
                    color: settingsModel && settingsModel.forecastStyle === "Dynamic" ? weatherLocation.iconColor : Kirigami.Theme.textColor
                    isMask: true
                }
                Column {
                    spacing: Kirigami.Units.smallSpacing
                    Label {
                        font.weight: Font.Bold
                        text: i18n("Humidity")
                        color: root.textColor
                    }
                    Label {
                        text: selectedDay == null ? "" : Formatter.formatPercent(selectedDay.humidity.toFixed(1))
                        color: root.textColor
                    }
                }
            }

            // Atmospheric pressure
            RowLayout {
                spacing: Kirigami.Units.gridUnit
                Kirigami.Icon {
                    source: "speedometer"
                    implicitHeight: Kirigami.Units.iconSizes.medium
                    implicitWidth: Kirigami.Units.iconSizes.medium
                    color: settingsModel && settingsModel.forecastStyle === "Dynamic" ? weatherLocation.iconColor : Kirigami.Theme.textColor
                    isMask: true
                }
                Column {
                    spacing: Kirigami.Units.smallSpacing
                    Label {
                        font.weight: Font.Bold
                        text: i18n("Pressure")
                        color: root.textColor
                    }
                    Label {
                        text: selectedDay == null ? "" : Formatter.formatPressure(selectedDay.pressure, settingsModel.pressureUnits)
                        color: root.textColor
                    }
                }
            }

            // UV Index
            RowLayout {
                spacing: Kirigami.Units.gridUnit
                Kirigami.Icon {
                    source: "compass"
                    implicitHeight: Kirigami.Units.iconSizes.medium
                    implicitWidth: Kirigami.Units.iconSizes.medium
                    color: settingsModel && settingsModel.forecastStyle === "Dynamic" ? weatherLocation.iconColor : Kirigami.Theme.textColor
                    isMask: true
                }
                Column {
                    spacing: Kirigami.Units.smallSpacing
                    Label {
                        font.weight: Font.Bold
                        text: i18n("UV Index")
                        color: root.textColor
                    }
                    Label {
                        text: selectedDay == null ? "" : Formatter.formatDouble(selectedDay.uvIndex.toFixed(1))
                        color: root.textColor
                    }
                }
            }
        }
    }
}
