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

    signal closeRequested()

    spacing: 0

    // HACK: dialog switching requires some time between closing and opening
    Timer {
        id: dialogTimer

        property var dialog

        interval: 1
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
                function onClosed() {
                    if (root.dialog)
                        root.dialog.open();

                }

                target: forecastStyleDropdown.dialog
            }

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

        }

        FormCard.FormDelegateSeparator {
            above: forecastStyleDropdown
            below: aboutButton
        }

        FormCard.FormButtonDelegate {
            id: aboutButton

            text: i18n("About")
            onClicked: {
                applicationWindow().pageStack.push(Qt.createComponent("org.kde.kirigamiaddons.formcard", "AboutPage"));
                if (root.dialog)
                    root.dialog.close();

            }
        }

    }

    FormCard.FormHeader {
        title: i18n("Units")
    }

    FormCard.FormCard {
        TemperatureUnitComboBox {
            id: temperatureUnitsDropdown

            onClicked: {
                if (root.dialog && temperatureUnitsDropdown.mode === FormCard.FormComboBoxDelegate.Dialog) {
                    dialogTimer.dialog = temperatureUnitsDropdown.dialog;
                    dialogTimer.restart();
                }
            }

            Connections {
                function onClosed() {
                    if (root.dialog)
                        root.dialog.open();

                }

                target: temperatureUnitsDropdown.dialog
            }

        }

        FormCard.FormDelegateSeparator {
            above: temperatureUnitsDropdown
            below: speedUnitsDropdown
        }

        SpeedUnitComboBox {
            id: speedUnitsDropdown

            onClicked: {
                if (root.dialog && speedUnitsDropdown.mode === FormCard.FormComboBoxDelegate.Dialog) {
                    dialogTimer.dialog = speedUnitsDropdown.dialog;
                    dialogTimer.restart();
                }
            }

            Connections {
                function onClosed() {
                    if (root.dialog)
                        root.dialog.open();

                }

                target: speedUnitsDropdown.dialog
            }

        }

        FormCard.FormDelegateSeparator {
            above: speedUnitsDropdown
            below: pressureUnitsDropdown
        }

        PressureUnitComboBox {
            id: pressureUnitsDropdown

            onClicked: {
                if (root.dialog && pressureUnitsDropdown.mode === FormCard.FormComboBoxDelegate.Dialog) {
                    dialogTimer.dialog = pressureUnitsDropdown.dialog;
                    dialogTimer.restart();
                }
            }

            Connections {
                function onClosed() {
                    if (root.dialog)
                        root.dialog.open();

                }

                target: pressureUnitsDropdown.dialog
            }

        }

        FormCard.FormDelegateSeparator {
            above: pressureUnitsDropdown
            below: precipitationUnitsDropdown
        }

        // Precipitation
        PrecipitationUnitComboBox {
            id: precipitationUnitsDropdown

            onClicked: {
                if (root.dialog && precipitationUnitsDropdown.mode === FormCard.FormComboBoxDelegate.Dialog) {
                    dialogTimer.dialog = precipitationUnitsDropdown.dialog;
                    dialogTimer.restart();
                }
            }

            Connections {
                function onClosed() {
                    if (root.dialog)
                        root.dialog.open();

                }

                target: precipitationUnitsDropdown.dialog
            }

        }

    }

}
