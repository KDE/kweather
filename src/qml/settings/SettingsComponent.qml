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
            displayMode: FormCard.FormComboBoxDelegate.Dialog

            onActivated: settingsModel.save()
            onCurrentValueChanged: settingsModel.forecastStyle = currentValue

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

        FormCard.FormDelegateSeparator {}

        FormCard.FormButtonDelegate {
            id: aboutButton

            text: i18n("About")
            onClicked: {
                applicationWindow().pageStack.push(Qt.createComponent("org.kde.kirigamiaddons.formcard", "AboutPage"));
                root.closeRequested();
            }
        }
    }

    FormCard.FormHeader {
        title: i18n("Units")
    }

    FormCard.FormCard {
        TemperatureUnitComboBox {
            id: temperatureUnitsDropdown
        }

        FormCard.FormDelegateSeparator {}

        SpeedUnitComboBox {
            id: speedUnitsDropdown
        }

        FormCard.FormDelegateSeparator {}

        PressureUnitComboBox {
            id: pressureUnitsDropdown
        }

        FormCard.FormDelegateSeparator {}

        // Precipitation
        PrecipitationUnitComboBox {
            id: precipitationUnitsDropdown
        }
    }
}
