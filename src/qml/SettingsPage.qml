/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2021 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.11 as Kirigami

Kirigami.ScrollablePage {
    id: settingsRoot
    title: i18n("Settings")
    
    property int yTranslate: 0

    topPadding: 0
    bottomPadding: 0
    leftPadding: 0
    rightPadding: 0
    Kirigami.ColumnView.fillWidth: false
    Kirigami.Theme.inherit: false
    Kirigami.Theme.colorSet: Kirigami.Theme.View

    ColumnLayout {
        transform: Translate { y: yTranslate }
        spacing: 0

        ItemDelegate {
            Layout.fillWidth: true
            implicitHeight: Kirigami.Units.gridUnit * 3
            onClicked: forecastStyle.open()

            ColumnLayout {
                spacing: -5
                anchors.leftMargin: Kirigami.Units.gridUnit
                anchors.rightMargin: Kirigami.Units.gridUnit
                anchors.fill: parent

                Label {
                    text: i18n("Forecast Style")
                    font.weight: Font.Bold
                }
                Label {
                    text: settingsModel.forecastStyle
                }
            }
        }

        Kirigami.Separator {
            Layout.fillWidth: true
            Layout.leftMargin: Kirigami.Units.largeSpacing
            Layout.rightMargin: Kirigami.Units.largeSpacing
            opacity: 0.8
        }
        
        ItemDelegate {
            Layout.fillWidth: true
            implicitHeight: Kirigami.Units.gridUnit * 3
            onClicked: temperatureUnits.open()

            ColumnLayout {
                spacing: -5
                anchors.leftMargin: Kirigami.Units.gridUnit
                anchors.rightMargin: Kirigami.Units.gridUnit
                anchors.fill: parent

                Label {
                    text: i18n("Temperature Units")
                    font.weight: Font.Bold
                }
                Label {
                    text: i18n(settingsModel.temperatureUnits)
                }
            }
        }

        Kirigami.Separator {
            Layout.fillWidth: true
            Layout.leftMargin: Kirigami.Units.largeSpacing
            Layout.rightMargin: Kirigami.Units.largeSpacing
            opacity: 0.8
        }

        ItemDelegate {
            Layout.fillWidth: true
            implicitHeight: Kirigami.Units.gridUnit * 3
            onClicked: speedUnits.open()

            ColumnLayout {
                spacing: -5
                anchors.leftMargin: Kirigami.Units.gridUnit
                anchors.rightMargin: Kirigami.Units.gridUnit
                anchors.fill: parent

                Label {
                    text: i18n("Speed Units")
                    font.weight: Font.Bold
                }
                Label {
                    text: settingsModel.speedUnits
                }
            }
        }

        Kirigami.Separator {
            Layout.fillWidth: true
            Layout.leftMargin: Kirigami.Units.largeSpacing
            Layout.rightMargin: Kirigami.Units.largeSpacing
            opacity: 0.8
        }

        ItemDelegate {
            Layout.fillWidth: true
            implicitHeight: Kirigami.Units.gridUnit * 3
            onClicked: pressureUnits.open()

            ColumnLayout {
                spacing: -5
                anchors.leftMargin: Kirigami.Units.gridUnit
                anchors.rightMargin: Kirigami.Units.gridUnit
                anchors.fill: parent

                Label {
                    text: i18n("Pressure Units")
                    font.weight: Font.Bold
                }
                Label {
                    text: settingsModel.pressureUnits
                }
            }
        }

        Kirigami.Separator {
            Layout.fillWidth: true
            Layout.leftMargin: Kirigami.Units.largeSpacing
            Layout.rightMargin: Kirigami.Units.largeSpacing
            opacity: 0.8
        }

        ItemDelegate {
            Layout.fillWidth: true
            implicitHeight: Kirigami.Units.gridUnit * 3
            
            onClicked: appwindow.pageStack.layers.push(getPage("About"))
            
            Label {
                anchors.left: parent.left
                anchors.leftMargin: Kirigami.Units.gridUnit
                anchors.verticalCenter: parent.verticalCenter
                font.weight: Font.Bold
                font.bold: true
                text: i18n("About")
            }
        }

        Kirigami.Separator {
            Layout.fillWidth: true
            Layout.leftMargin: Kirigami.Units.largeSpacing
            Layout.rightMargin: Kirigami.Units.largeSpacing
            opacity: 0.8
        }
        
        // forecast style sheet
        SettingsDialog {
            id: forecastStyle
            title: i18n("Forecast Style")
            options: [["Flat", i18n("Flat")], ["Dynamic", i18n("Dynamic")]]
            settingName: "forecastStyle"
        }
        
        // temperature unit dialog
        SettingsDialog {
            id: temperatureUnits
            title: i18n("Temperature Units")
            options: [["Use System Default", i18n("Use System Default")], ["Celsius", i18n("Celsius")], ["Fahrenheit", i18n("Fahrenheit")]]
            settingName: "temperatureUnits"
        }

        // speed unit dialog
        SettingsDialog {
            id: speedUnits
            title: i18n("Speed Units")
            options: [["kph", i18nc("kilometers per hour", "kph")], ["mph", i18nc("miles per hour", "mph")], ["m/s", i18nc("meters per second", "m/s")]]
            settingName: "speedUnits"
        }

        // pressure unit dialog
        SettingsDialog {
            id: pressureUnits
            title: i18n("Pressure Units")
            options: [["hPa", i18nc("Hectopascal Pressure", "hPa")], ["mmHg", i18nc("Millimetre of mercury", "mmHg")]]
            settingName: "pressureUnits"

        }
    }
}
