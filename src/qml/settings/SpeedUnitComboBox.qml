// SPDX-FileCopyrightText: 2020-2024 Devin Lin <espidev@gmail.com>
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Controls
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

FormCard.FormComboBoxDelegate {
    id: root

    text: i18n("Speed Units")
    currentIndex: indexOfValue(settingsModel.speedUnits)
    textRole: "name"
    valueRole: "value"
    onActivated: settingsModel.save()
    onCurrentValueChanged: settingsModel.speedUnits = currentValue

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
            root.currentIndex = root.indexOfValue(settingsModel.speedUnits);
        }
    }

}
