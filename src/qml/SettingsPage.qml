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
                    text: settingsModel.OWMToken.length == 0 ? i18n("Not Set") : i18n("Added")
                }
            }
        }

        Kirigami.Separator {
            Layout.fillWidth: true
        }

        ItemDelegate {
            Layout.fillWidth: true
            font.bold: true
            text: i18n("About")
            leftPadding: Kirigami.Units.gridUnit
            rightPadding: Kirigami.Units.gridUnit
            implicitHeight: Kirigami.Units.gridUnit * 3
            onClicked: pageStack.push(aboutPage)
        }

        Kirigami.Separator {
            Layout.fillWidth: true
        }
    }

    // temperature unit dialog
    Dialog {
        id: temperatureUnits
        modal: true
        focus: true
        x: (parent.width - width) / 2
        y: parent.height / 2 - height
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
        x: (parent.width - width) / 2
        y: parent.height / 2 - height
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
        x: (parent.width - width) / 2
        y: parent.height / 2 - height
        width: Math.min(parent.width - Kirigami.Units.gridUnit * 4, Kirigami.Units.gridUnit * 20)
        height: Kirigami.Units.gridUnit * 10
        title: i18n("OpenWeatherMap API Token")
        standardButtons: Dialog.Close | Dialog.Save

        onAccepted: settingsModel.OWMToken = textField.text;

        contentItem: ColumnLayout {
            Layout.fillWidth: true
            spacing: Kirigami.Units.smallSpacing
            Label {
                color: Kirigami.Theme.disabledTextColor
                text: i18n("Add your own OpenWeatherMap API token.")
            }
            Label {
                color: Kirigami.Theme.disabledTextColor
                text: i18n("Token added is stored as PLAIN TEXT in system.")
            }
            Kirigami.Separator {}
            TextField {
                id: textField
                placeholderText: settingsModel.OWMToken.length == 0 ? i18n("Add your API token here... ") : settingsModel.OWMToken
                anchors.top: parent.Top
                width: parent.width
                Layout.fillWidth: true
            }
            Kirigami.Separator {}
        }
        Component.onCompleted: background.visible = true
    }
    
}
