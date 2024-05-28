/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2022 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

ColumnLayout {
    id: root
    property var dialog: null // dialog component if this is within a dialog

    spacing: 0

    signal closeRequested

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

    FormCard.FormHeader {
        title: i18n("General")
    }

    FormCard.FormCard {
        FormCard.FormComboBoxDelegate {
            id: forecastStyleDropdown
            text: i18n("Forecast Style")
            currentIndex: indexOfValue(settingsModel.forecastStyle)
            model: ListModel {
                // we can't use i18n with ListElement
                Component.onCompleted: {
                    append({
                        "name": i18n("Flat"),
                        "value": "Flat"
                    });
                    append({
                        "name": i18n("Dynamic"),
                        "value": "Dynamic"
                    });

                    // indexOfValue doesn't bind to model changes unfortunately, set currentIndex manually here
                    forecastStyleDropdown.currentIndex = forecastStyleDropdown.indexOfValue(settingsModel.forecastStyle);
                }
            }

            textRole: "name"
            valueRole: "value"
            onActivated: settingsModel.save()
            onCurrentValueChanged: settingsModel.forecastStyle = currentValue

            onClicked: {
                if (root.dialog && forecastStyleDropdown.mode === FormCard.FormComboBoxDelegate.Dialog) {
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

        FormCard.FormDelegateSeparator {
            above: forecastStyleDropdown
            below: aboutButton
        }

        FormCard.FormButtonDelegate {
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

    FormCard.FormHeader {
        title: i18n("Units")
    }

    FormCard.FormCard {
        FormCard.FormComboBoxDelegate {
            id: temperatureUnitsDropdown
            text: i18n("Temperature Units")
            currentIndex: indexOfValue(settingsModel.temperatureUnits)
            model: ListModel {
                // we can't use i18n with ListElement
                Component.onCompleted: {
                    append({
                        "name": i18n("Use System Default"),
                        "value": "Use System Default"
                    });
                    append({
                        "name": i18n("Celsius"),
                        "value": "Celsius"
                    });
                    append({
                        "name": i18n("Fahrenheit"),
                        "value": "Fahrenheit"
                    });

                    // indexOfValue doesn't bind to model changes unfortunately, set currentIndex manually here
                    temperatureUnitsDropdown.currentIndex = temperatureUnitsDropdown.indexOfValue(settingsModel.temperatureUnits);
                }
            }

            textRole: "name"
            valueRole: "value"
            onActivated: settingsModel.save()
            onCurrentValueChanged: settingsModel.temperatureUnits = currentValue

            onClicked: {
                if (root.dialog && temperatureUnitsDropdown.mode === FormCard.FormComboBoxDelegate.Dialog) {
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

        FormCard.FormDelegateSeparator {
            above: temperatureUnitsDropdown
            below: speedUnitsDropdown
        }

        FormCard.FormComboBoxDelegate {
            id: speedUnitsDropdown
            text: i18n("Speed Units")
            currentIndex: indexOfValue(settingsModel.speedUnits)
            model: ListModel {
                // we can't use i18n with ListElement
                Component.onCompleted: {
                    append({
                        "name": i18nc("kilometers per hour", "kph"),
                        "value": "kph"
                    });
                    append({
                        "name": i18nc("miles per hour", "mph"),
                        "value": "mph"
                    });
                    append({
                        "name": i18nc("meters per second", "m/s"),
                        "value": "m/s"
                    });

                    // indexOfValue doesn't bind to model changes unfortunately, set currentIndex manually here
                    speedUnitsDropdown.currentIndex = speedUnitsDropdown.indexOfValue(settingsModel.speedUnits);
                }
            }

            textRole: "name"
            valueRole: "value"
            onActivated: settingsModel.save()
            onCurrentValueChanged: settingsModel.speedUnits = currentValue

            onClicked: {
                if (root.dialog && speedUnitsDropdown.mode === FormCard.FormComboBoxDelegate.Dialog) {
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

        FormCard.FormDelegateSeparator {
            above: speedUnitsDropdown
            below: pressureUnitsDropdown
        }

        FormCard.FormComboBoxDelegate {
            id: pressureUnitsDropdown
            text: i18n("Pressure Units")
            currentIndex: indexOfValue(settingsModel.pressureUnits)
            model: ListModel {
                // we can't use i18n with ListElement
                Component.onCompleted: {
                    append({
                        "name": i18nc("Hectopascal Pressure", "hPa"),
                        "value": "hPa"
                    });
                    append({
                        "name": i18nc("Millimetre of mercury", "mmHg"),
                        "value": "mmHg"
                    });

                    // indexOfValue doesn't bind to model changes unfortunately, set currentIndex manually here
                    pressureUnitsDropdown.currentIndex = pressureUnitsDropdown.indexOfValue(settingsModel.pressureUnits);
                }
            }

            textRole: "name"
            valueRole: "value"
            onActivated: settingsModel.save()
            onCurrentValueChanged: settingsModel.pressureUnits = currentValue

            onClicked: {
                if (root.dialog && pressureUnitsDropdown.mode === FormCard.FormComboBoxDelegate.Dialog) {
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

        FormCard.FormDelegateSeparator {
            above: pressureUnitsDropdown
            below: precipitationUnitsDropdown
        }

        // Precipitation
        FormCard.FormComboBoxDelegate {
            id: precipitationUnitsDropdown
            text: i18n("Precipitation Units")
            currentIndex: indexOfValue(settingsModel.precipitationUnits)
            model: ListModel {
                // we can't use i18n with ListElement
                Component.onCompleted: {
                    append({
                        "name": i18nc("Millimeters", "mm"),
                        "value": "mm"
                    });
                    append({
                        "name": i18nc("Inches", "in"),
                        "value": "in"
                    });

                    // indexOfValue doesn't bind to model changes unfortunately, set currentIndex manually here
                    precipitationUnitsDropdown.currentIndex = precipitationUnitsDropdown.indexOfValue(settingsModel.precipitationUnits);
                }
            }

            textRole: "name"
            valueRole: "value"
            onActivated: settingsModel.save()
            onCurrentValueChanged: settingsModel.precipitationUnits = currentValue

            onClicked: {
                if (root.dialog && precipitationUnitsDropdown.mode === FormCard.FormComboBoxDelegate.Dialog) {
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
