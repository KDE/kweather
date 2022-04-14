/*
 * Copyright 2022 Han Young <hanyoung@protonmail.com>
 * Copyright 2022 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.11 as Kirigami

PopupDialog {
    standardButtons: Dialog.Close
    property var options
    property string settingName

    ColumnLayout {
        Kirigami.Theme.inherit: false
        Kirigami.Theme.colorSet: Kirigami.Theme.View
        spacing: 0

        Repeater {
            model: options
            delegate: RadioDelegate {
                topPadding: Kirigami.Units.smallSpacing * 2
                bottomPadding: Kirigami.Units.smallSpacing * 2
                implicitWidth: Kirigami.Units.gridUnit * 16

                text: modelData[1]
                checked: settingsModel[settingName] == modelData[0]
                onCheckedChanged: {
                    if (checked) {
                        settingsModel[settingName] = modelData[0];
                        settingsModel.save();
                    }
                }
            }
        }
    }
}
