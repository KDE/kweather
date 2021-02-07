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

    topPadding: 0
    bottomPadding: 0
    leftPadding: 0
    rightPadding: 0
    Kirigami.ColumnView.fillWidth: false

    ColumnLayout {
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
        }


        ItemDelegate {
            Layout.fillWidth: true
            implicitHeight: Kirigami.Units.gridUnit * 3
            onClicked: apiToken.open()

            ColumnLayout {
                spacing: -5
                anchors.leftMargin: Kirigami.Units.gridUnit
                anchors.rightMargin: Kirigami.Units.gridUnit
                anchors.fill: parent

                Label {
                    text: i18n("API Token")
                    font.weight: Font.Bold
                }
                Label {
                    text: settingsModel.OWMToken == undefined || settingsModel.OWMToken.length == 0 ? i18n("Not Set") : i18n("Added")
                }
            }
        }

        Kirigami.Separator {
            Layout.fillWidth: true
        }

        ItemDelegate {
            Layout.fillWidth: true
            implicitHeight: Kirigami.Units.gridUnit * 3
            
            onClicked: appwindow.pageStack.push(aboutPage)
            
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
        }
    }

    // forecast style sheet
    Kirigami.OverlaySheet {
        id: forecastStyle
        parent: applicationWindow().overlay
        
        header: Kirigami.Heading {
            text: i18n("Forecast Style")
        }
        
        footer: RowLayout {
            Button {
                text: i18n("Close")
                Layout.alignment: Qt.AlignRight
                onClicked: forecastStyle.close()
            }
        }
        
        ListView {
            Layout.leftMargin: Kirigami.Units.gridUnit
            Layout.preferredWidth: settingsRoot.width - Kirigami.Units.gridUnit * 8
            model: [i18n("Flat"), i18n("Dynamic")]
            delegate: RadioDelegate {
                width: parent.width
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
    
    // temperature unit dialog
    Dialog {
        id: temperatureUnits
        modal: true
        focus: true
        anchors.centerIn: Overlay.overlay
        width: Math.min(parent.width - Kirigami.Units.gridUnit * 4, Kirigami.Units.gridUnit * 20)
        height: Kirigami.Units.gridUnit * 20
        title: i18n("Temperature Units")
        standardButtons: Dialog.Close

        onAccepted: weatherLocationListModel.updateUi();
        onRejected: weatherLocationListModel.updateUi();

        contentItem: ScrollView {
            ListView {
                model: [i18n("Celsius"), i18n("Fahrenheit")]
                delegate: RadioDelegate {
                    width: parent.width
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
            Component.onCompleted: background.visible = true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        }
    }

    // speed unit dialog
    Dialog {
        id: speedUnits
        modal: true
        focus: true
        anchors.centerIn: Overlay.overlay
        width: Math.min(parent.width - Kirigami.Units.gridUnit * 4, Kirigami.Units.gridUnit * 20)
        height: Kirigami.Units.gridUnit * 20
        title: i18n("Speed Units")
        standardButtons: Dialog.Close

        onAccepted: weatherLocationListModel.updateUi();
        onRejected: weatherLocationListModel.updateUi();

        contentItem: ScrollView {
            ListView {
                model: [i18nc("kilometers per hour", "kph"), i18nc("miles per hour", "mph")]
                delegate: RadioDelegate {
                    width: parent.width
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
            Component.onCompleted: background.visible = true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        }
    }

    // API Token dialog
    Dialog {
        id: apiToken
        modal: true
        focus: true
        anchors.centerIn: Overlay.overlay
        width: Math.min(parent.width * 0.8, Kirigami.Units.gridUnit * 20)
        height: Kirigami.Units.gridUnit * 12
        title: i18n("OpenWeatherMap API Token")
        standardButtons: Dialog.Close | Dialog.Save

        onAccepted: {
            settingsModel.OWMToken = textField.text
            settingsModel.save()
        }

        contentItem: ColumnLayout {
            width: apiToken.width - apiToken.leftPadding * 2
            spacing: Kirigami.Units.smallSpacing
            Label {
                Layout.preferredWidth: parent.width
                wrapMode: Text.WrapAnywhere
                color: Kirigami.Theme.disabledTextColor
                text: i18n("Add your own OpenWeatherMap API token.")
            }
            Label {
                Layout.preferredWidth: parent.width
                wrapMode: Text.WrapAnywhere
                color: Kirigami.Theme.disabledTextColor
                text: i18n("Token added is stored as PLAIN TEXT in system.")
            }
            Kirigami.Separator {}
            TextField {
                id: textField
                Layout.preferredWidth: parent.width
                wrapMode: Text.WrapAnywhere
                placeholderText: settingsModel.OWMToken == undefined || settingsModel.OWMToken.length == 0 ? i18n("Add your API token here... ") : settingsModel.OWMToken
                anchors.top: parent.Top
            }
            Kirigami.Separator {}
        }
        Component.onCompleted: background.visible = true
    }
    
}
