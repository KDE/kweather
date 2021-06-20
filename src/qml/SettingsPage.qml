/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
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
                    text: settingsModel.temperatureUnits
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
            
            onClicked: appwindow.pageStack.push(getPage("About"))
            
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
        PopupDialog {
            id: forecastStyle
            standardButtons: Dialog.Close
            title: i18n("Forecast Style")
            
            ColumnLayout {
                Kirigami.Theme.inherit: false
                Kirigami.Theme.colorSet: Kirigami.Theme.View
                spacing: 0
                
                Repeater {
                    model: [i18n("Flat"), i18n("Dynamic")]
                    delegate: RadioDelegate {
                        topPadding: Kirigami.Units.smallSpacing * 2
                        bottomPadding: Kirigami.Units.smallSpacing * 2
                        implicitWidth: Kirigami.Units.gridUnit * 16
                        
                        text: modelData
                        checked: settingsModel.forecastStyle == modelData
                        onCheckedChanged: {
                            if (checked) {
                                settingsModel.forecastStyle = modelData;
                                settingsModel.save();
                            }
                        }
                    }
                }
            }
        }
        
        // temperature unit dialog
        PopupDialog {
            id: temperatureUnits
            standardButtons: Dialog.Close
            title: i18n("Temperature Units")

            onAccepted: weatherLocationListModel.updateUi();
            onRejected: weatherLocationListModel.updateUi();

            ColumnLayout {
                Kirigami.Theme.inherit: false
                Kirigami.Theme.colorSet: Kirigami.Theme.View
                spacing: 0
                
                Repeater {
                    model: [i18n("Celsius"), i18n("Fahrenheit")]
                    delegate: RadioDelegate {
                        topPadding: Kirigami.Units.smallSpacing * 2
                        bottomPadding: Kirigami.Units.smallSpacing * 2
                        implicitWidth: Kirigami.Units.gridUnit * 16
                        
                        text: modelData
                        checked: settingsModel.temperatureUnits == modelData
                        onCheckedChanged: {
                            if (checked) {
                                settingsModel.temperatureUnits = modelData;
                                settingsModel.save()
                            }
                        }
                    }
                }
            }
        }

        // speed unit dialog
        PopupDialog {
            id: speedUnits
            title: i18n("Speed Units")
            standardButtons: Dialog.Close

            onAccepted: weatherLocationListModel.updateUi();
            onRejected: weatherLocationListModel.updateUi();

            ColumnLayout {
                Kirigami.Theme.inherit: false
                Kirigami.Theme.colorSet: Kirigami.Theme.View
                spacing: 0
                
                Repeater {
                    model: [i18nc("kilometers per hour", "kph"), i18nc("miles per hour", "mph")]
                    delegate: RadioDelegate {
                        topPadding: Kirigami.Units.smallSpacing * 2
                        bottomPadding: Kirigami.Units.smallSpacing * 2
                        implicitWidth: Kirigami.Units.gridUnit * 16
                        
                        text: modelData
                        checked: settingsModel.speedUnits == modelData
                        onCheckedChanged: {
                            if (checked) {
                                settingsModel.speedUnits = modelData;
                                settingsModel.save()
                            }
                        }
                    }
                }
            }
        }
    }
}
