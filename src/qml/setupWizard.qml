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

    ColumnLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.alignment: Qt.AlignHCenter
        RowLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true

            Image {
                source: "qrc:/resources/flat.jpg"
                width: parent.width * 0.4
            }

            Image {
                source: "qrc:/resources/animated.jpg"
                width: parent.width * 0.4
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            RadioButton {
                text: i18n("flat")
                checked: true
            }

            RadioButton {
                text: i18n("animated")
            }
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
