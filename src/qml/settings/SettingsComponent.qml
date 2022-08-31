/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2022 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2

import org.kde.kirigami 2.11 as Kirigami
import org.kde.kirigamiaddons.labs.mobileform 0.1 as MobileForm

ColumnLayout {
    id: root
    property var dialog: null // dialog component if this is within a dialog

    spacing: 0
    
    signal closeRequested()
    
    // HACK: dialog switching requires some time between closing and opening
    Timer {
        id: dialogTimer
        interval: 1
        property var dialog
        onTriggered: {
            root.dialog.close();
            dialog.open();
        }
    }
    
    MobileForm.FormCard {
        Layout.fillWidth: true
        
        contentItem: ColumnLayout {
            spacing: 0
            
            MobileForm.FormCardHeader {
                title: i18n("General")
            }
            
            MobileForm.FormComboBoxDelegate {
                id: forecastStyleDropdown
                text: i18n("Forecast Style")
                currentValue: settingsModel.forecastStyle
                model: [["Flat", i18n("Flat")], ["Dynamic", i18n("Dynamic")]]
                
                onClicked: {
                    if (root.dialog) {
                        dialogTimer.dialog = forecastStyleDropdown.dialog;
                        dialogTimer.restart();
                    }
                }
                
                Connections {
                    target: forecastStyleDropdown.dialog
                    function onClosed() {
                        if (root.dialog) {
                            root.dialog.open();
                        }
                    }
                }
                
                dialogDelegate: RadioDelegate {
                    implicitWidth: Kirigami.Units.gridUnit * 16
                    topPadding: Kirigami.Units.smallSpacing * 2
                    bottomPadding: Kirigami.Units.smallSpacing * 2
                    
                    text: modelData[1]
                    checked: settingsModel.forecastStyle == modelData[0]
                    onCheckedChanged: {
                        if (checked) {
                            settingsModel.forecastStyle = modelData[0];
                            settingsModel.save();
                        }
                    }
                }
            }
            
            MobileForm.FormDelegateSeparator { above: forecastStyleDropdown; below: aboutButton }
            
            MobileForm.FormButtonDelegate {
                id: aboutButton
                text: i18n("About")
                onClicked: {
                    applicationWindow().pageStack.push(getPage("About"));
                    
                    if (root.dialog) {
                        root.dialog.close();
                    }
                }
            }
        }
    }
    
    MobileForm.FormCard {
        Layout.fillWidth: true
        Layout.topMargin: Kirigami.Units.largeSpacing
        
        contentItem: ColumnLayout {
            spacing: 0
            
            MobileForm.FormCardHeader {
                title: i18n("Units")
            }
            
            MobileForm.FormComboBoxDelegate {
                id: temperatureUnitsDropdown
                text: i18n("Temperature Units")
                currentValue: settingsModel.temperatureUnits
                model: [["Use System Default", i18n("Use System Default")], ["Celsius", i18n("Celsius")], ["Fahrenheit", i18n("Fahrenheit")]]
                
                onClicked: {
                    if (root.dialog) {
                        dialogTimer.dialog = temperatureUnitsDropdown.dialog;
                        dialogTimer.restart();
                    }
                }
                
                Connections {
                    target: temperatureUnitsDropdown.dialog
                    function onClosed() {
                        if (root.dialog) {
                            root.dialog.open();
                        }
                    }
                }
                
                dialogDelegate: RadioDelegate {
                    implicitWidth: Kirigami.Units.gridUnit * 16
                    topPadding: Kirigami.Units.smallSpacing * 2
                    bottomPadding: Kirigami.Units.smallSpacing * 2
                    
                    text: modelData[1]
                    checked: settingsModel.temperatureUnits == modelData[0]
                    onCheckedChanged: {
                        if (checked) {
                            settingsModel.temperatureUnits = modelData[0];
                            settingsModel.save();
                        }
                    }
                }
            }
            
            MobileForm.FormDelegateSeparator { above: temperatureUnitsDropdown; below: speedUnitsDropdown }
            
            MobileForm.FormComboBoxDelegate {
                id: speedUnitsDropdown
                text: i18n("Speed Units")
                currentValue: settingsModel.speedUnits
                model: [["kph", i18nc("kilometers per hour", "kph")], ["mph", i18nc("miles per hour", "mph")], ["m/s", i18nc("meters per second", "m/s")]]
                
                onClicked: {
                    if (root.dialog) {
                        dialogTimer.dialog = speedUnitsDropdown.dialog;
                        dialogTimer.restart();
                    }
                }
                
                Connections {
                    target: speedUnitsDropdown.dialog
                    function onClosed() {
                        if (root.dialog) {
                            root.dialog.open();
                        }
                    }
                }
                
                dialogDelegate: RadioDelegate {
                    implicitWidth: Kirigami.Units.gridUnit * 16
                    topPadding: Kirigami.Units.smallSpacing * 2
                    bottomPadding: Kirigami.Units.smallSpacing * 2
                    
                    text: modelData[1]
                    checked: settingsModel.speedUnits == modelData[0]
                    onCheckedChanged: {
                        if (checked) {
                            settingsModel.speedUnits = modelData[0];
                            settingsModel.save();
                        }
                    }
                }
            }
            
            MobileForm.FormDelegateSeparator { above: speedUnitsDropdown; below: pressureUnitsDropdown }
            
            MobileForm.FormComboBoxDelegate {
                id: pressureUnitsDropdown
                text: i18n("Pressure Units")
                currentValue: settingsModel.pressureUnits
                model: [["hPa", i18nc("Hectopascal Pressure", "hPa")], ["mmHg", i18nc("Millimetre of mercury", "mmHg")]]
                
                onClicked: {
                    if (root.dialog) {
                        dialogTimer.dialog = pressureUnitsDropdown.dialog;
                        dialogTimer.restart();
                    }
                }
                
                Connections {
                    target: pressureUnitsDropdown.dialog
                    function onClosed() {
                        if (root.dialog) {
                            root.dialog.open();
                        }
                    }
                }
                
                dialogDelegate: RadioDelegate {
                    implicitWidth: Kirigami.Units.gridUnit * 16
                    topPadding: Kirigami.Units.smallSpacing * 2
                    bottomPadding: Kirigami.Units.smallSpacing * 2
                    
                    text: modelData[1]
                    checked: settingsModel.pressureUnits == modelData[0]
                    onCheckedChanged: {
                        if (checked) {
                            settingsModel.pressureUnits = modelData[0];
                            settingsModel.save();
                        }
                    }
                }
            }
        }
    }
}
