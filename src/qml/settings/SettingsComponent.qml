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
                currentIndex: indexOfValue(settingsModel.forecastStyle)
                model: ListModel {
                    // we can't use i18n with ListElement
                    Component.onCompleted: {
                        append({"name": i18n("Flat"), "value": "Flat"});
                        append({"name": i18n("Dynamic"), "value": "Dynamic"});
                        
                        // indexOfValue doesn't bind to model changes unfortunately, set currentIndex manually here
                        forecastStyleDropdown.currentIndex = forecastStyleDropdown.indexOfValue(settingsModel.forecastStyle)
                    }
                }
                
                textRole: "name"
                valueRole: "value"
                onActivated: settingsModel.save()
                onCurrentValueChanged: settingsModel.forecastStyle = currentValue;
                
                onClicked: {
                    if (root.dialog && forecastStyleDropdown.mode === MobileForm.FormComboBoxDelegate.Dialog) {
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
                currentIndex: indexOfValue(settingsModel.temperatureUnits)
                model: ListModel {
                    // we can't use i18n with ListElement
                    Component.onCompleted: {
                        append({"name": i18n("Use System Default"), "value": "Use System Default"});
                        append({"name": i18n("Celsius"), "value": "Celsius"});
                        append({"name": i18n("Fahrenheit"), "value": "Fahrenheit"});
                        
                        // indexOfValue doesn't bind to model changes unfortunately, set currentIndex manually here
                        temperatureUnitsDropdown.currentIndex = temperatureUnitsDropdown.indexOfValue(settingsModel.temperatureUnits)
                    }
                }
                
                textRole: "name"
                valueRole: "value"
                onActivated: settingsModel.save()
                onCurrentValueChanged: settingsModel.temperatureUnits = currentValue;
                
                onClicked: {
                    if (root.dialog && temperatureUnitsDropdown.mode === MobileForm.FormComboBoxDelegate.Dialog) {
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
            }
            
            MobileForm.FormDelegateSeparator { above: temperatureUnitsDropdown; below: speedUnitsDropdown }
            
            MobileForm.FormComboBoxDelegate {
                id: speedUnitsDropdown
                text: i18n("Speed Units")
                currentIndex: indexOfValue(settingsModel.speedUnits)
                model: ListModel {
                    // we can't use i18n with ListElement
                    Component.onCompleted: {
                        append({"name": i18nc("kilometers per hour", "kph"), "value": "kph"});
                        append({"name": i18nc("miles per hour", "mph"), "value": "mph"});
                        append({"name": i18nc("meters per second", "m/s"), "value": "m/s"});
                        
                        // indexOfValue doesn't bind to model changes unfortunately, set currentIndex manually here
                        speedUnitsDropdown.currentIndex = speedUnitsDropdown.indexOfValue(settingsModel.speedUnits)
                    }
                }
                
                textRole: "name"
                valueRole: "value"
                onActivated: settingsModel.save()
                onCurrentValueChanged: settingsModel.speedUnits = currentValue;
                
                onClicked: {
                    if (root.dialog && speedUnitsDropdown.mode === MobileForm.FormComboBoxDelegate.Dialog) {
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
            }
            
            MobileForm.FormDelegateSeparator { above: speedUnitsDropdown; below: pressureUnitsDropdown }
            
            MobileForm.FormComboBoxDelegate {
                id: pressureUnitsDropdown
                text: i18n("Pressure Units")
                currentIndex: indexOfValue(settingsModel.pressureUnits)
                model: ListModel {
                    // we can't use i18n with ListElement
                    Component.onCompleted: {
                        append({"name": i18nc("Hectopascal Pressure", "hPa"), "value": "hPa"});
                        append({"name": i18nc("Millimetre of mercury", "mmHg"), "value": "mmHg"});
                        
                        // indexOfValue doesn't bind to model changes unfortunately, set currentIndex manually here
                        pressureUnitsDropdown.currentIndex = pressureUnitsDropdown.indexOfValue(settingsModel.pressureUnits)
                    }
                }
                
                textRole: "name"
                valueRole: "value"
                onActivated: settingsModel.save()
                onCurrentValueChanged: settingsModel.pressureUnits = currentValue;
                
                onClicked: {
                    if (root.dialog && pressureUnitsDropdown.mode === MobileForm.FormComboBoxDelegate.Dialog) {
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
            }
            
            MobileForm.FormDelegateSeparator { above: pressureUnitsDropdown; below: precipitationUnitsDropdown }

            // Precipitation
            MobileForm.FormComboBoxDelegate {
                id: precipitationUnitsDropdown
                text: i18n("Precipitation Units")
                currentIndex: indexOfValue(settingsModel.precipitationUnits)
                model: ListModel {
                    // we can't use i18n with ListElement
                    Component.onCompleted: {
                        append({"name": i18nc("Millimeters", "mm"), "value": "mm"});
                        append({"name": i18nc("Inches", "in"), "value": "in"});
                        
                        // indexOfValue doesn't bind to model changes unfortunately, set currentIndex manually here
                        precipitationUnitsDropdown.currentIndex = precipitationUnitsDropdown.indexOfValue(settingsModel.precipitationUnits)
                    }
                }
                
                textRole: "name"
                valueRole: "value"
                onActivated: settingsModel.save()
                onCurrentValueChanged: settingsModel.precipitationUnits = currentValue;
                
                onClicked: {
                    if (root.dialog && precipitationUnitsDropdown.mode === MobileForm.FormComboBoxDelegate.Dialog) {
                        dialogTimer.dialog = precipitationUnitsDropdown.dialog;
                        dialogTimer.restart();
                    }
                }

                Connections {
                    target: precipitationUnitsDropdown.dialog
                    function onClosed() {
                        if (root.dialog) {
                            root.dialog.open();
                        }
                    }
                }
            }
        }
    }
}
