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
import org.kde.kirigamiaddons.labs.mobileform 0.1 as MobileForm

Kirigami.ScrollablePage {
    id: root
    title: i18n("Settings")
    
    property int yTranslate: 0

    topPadding: Kirigami.Units.largeSpacing
    bottomPadding: Kirigami.Units.largeSpacing
    leftPadding: 0
    rightPadding: 0
    
    Kirigami.ColumnView.fillWidth: false
    Kirigami.Theme.inherit: false
    Kirigami.Theme.colorSet: Kirigami.Theme.Window

    ColumnLayout {
        transform: Translate { y: yTranslate }
        spacing: 0
        width: root.width
        
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
                    onClicked: applicationWindow().pageStack.layers.push(getPage("About"))
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
}
