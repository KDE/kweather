/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.11 as Kirigami

Kirigami.ApplicationWindow
{
    id: appwindow
    title: i18n("Weather")

    width: Kirigami.Units.gridUnit * 27
    height: Kirigami.Units.gridUnit * 45

    ButtonGroup {
        id: radioButton
    }

    ColumnLayout {
        Image {
            id: dynamicImage
            source: "qrc:/resources/KWeather_DYNAMIC.png"
            width: parent.width * 0.4
        }
        RadioButton {
            text: i18n("Dynamic")
            checked: true
            ButtonGroup.group: radioButton
        }
    }

    ColumnLayout {
        anchors.right: parent.right
        Image {
            source: "qrc:/resources/KWeather_FLAT.png"
            width: parent.width * 0.4
        }
        RadioButton {
            text: i18n("Flat")
            ButtonGroup.group: radioButton
        }
    }

    footer: Item {
        height: Kirigami.Units.gridUnit + confirmBtn.height
        Button {
            id: confirmBtn
            anchors.right: parent.right
            text: i18n("next")
            onClicked: appwindow.close()
        }
    }
}
