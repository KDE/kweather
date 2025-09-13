// SPDX-FileCopyrightText: 2020-2024 Devin Lin <espidev@gmail.com>
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Controls
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.formcard as FormCard

FormCard.FormComboBoxDelegate {
    id: root

    text: i18n("Precipitation Units")
    currentIndex: indexOfValue(settingsModel.precipitationUnits)
    textRole: "name"
    valueRole: "value"
    displayMode: FormCard.FormComboBoxDelegate.Dialog

    onActivated: settingsModel.save()
    onCurrentValueChanged: settingsModel.precipitationUnits = currentValue

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
            root.currentIndex = root.indexOfValue(settingsModel.precipitationUnits);
        }
    }

}
