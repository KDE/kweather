/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtCharts 2.3
import org.kde.kirigami 2.13 as Kirigami
import "backgrounds"

Kirigami.Card {

    id: root

    property var textColor: Kirigami.Theme.textColor

    contentItem: Item {
        implicitHeight: column.height
        Column {
            id: column
            spacing: Kirigami.Units.largeSpacing * 2

            // precipitation
            RowLayout {
                spacing: Kirigami.Units.largeSpacing
                Kirigami.Icon {
                    source: "raindrop"
                    Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                    Layout.minimumWidth: Layout.minimumHeight * 1.5
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
                        text: selectedDay == null ? "" : selectedDay.precipitation.toFixed(1) + "mm"
                        color: root.textColor
                    }
                }
            }

            // Humidity
            RowLayout {
                spacing: Kirigami.Units.largeSpacing
                Kirigami.Icon {
                    source: "compass"
                    Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                    Layout.minimumWidth: Layout.minimumHeight * 1.5
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
                        text: selectedDay == null ? "" : i18n("%1%", selectedDay.humidity.toFixed(1))
                        color: root.textColor
                    }
                }
            }

            // Atmospheric pressure
            RowLayout {
                spacing: Kirigami.Units.largeSpacing
                Kirigami.Icon {
                    source: "speedometer"
                    Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                    Layout.minimumWidth: Layout.minimumHeight * 1.5
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
                        text: selectedDay == null ? "" : i18n("%1hPa", selectedDay.pressure.toFixed(1))
                        color: root.textColor
                    }
                }
            }

            // UV Index
            RowLayout {
                spacing: Kirigami.Units.largeSpacing
                Kirigami.Icon {
                    source: "compass"
                    Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                    Layout.minimumWidth: Layout.minimumHeight * 1.5
                    color: settingsModel && settingsModel.forecastStyle === "Dynamic" ? weatherLocation.iconColor : Kirigami.Theme.textColor
                    isMask: true
                }
                Column {
                    spacing: Kirigami.Units.smallSpacing
                    Label {
                        font.weight: Font.Bold
                        text: i18n("UV index")
                        color: root.textColor
                    }
                    Label {
                        text: selectedDay == null ? "" : selectedDay.uvIndex.toFixed(1)
                        color: root.textColor
                    }
                }
            }
        }
    }
}
