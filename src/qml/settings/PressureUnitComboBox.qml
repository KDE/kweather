// SPDX-FileCopyrightText: 2020-2024 Devin Lin <espidev@gmail.com>
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Controls
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

FormCard.FormComboBoxDelegate {
    id: root

    text: i18n("Pressure Units")
    currentIndex: indexOfValue(settingsModel.pressureUnits)
    textRole: "name"
    valueRole: "value"
    displayMode: FormCard.FormComboBoxDelegate.Dialog

    onActivated: settingsModel.save()
    onCurrentValueChanged: settingsModel.pressureUnits = currentValue

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
            root.currentIndex = root.indexOfValue(settingsModel.pressureUnits);
        }
    }

}
