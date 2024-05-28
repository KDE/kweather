/*
    SPDX-FileCopyrightText: 2021 HanY <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import org.kde.plasma.plasmoid
import org.kde.kirigami as Kirigami
import org.kde.kirigamiaddons.delegates as Delegates

Rectangle {
    id: container

    signal selected

    color: Kirigami.Theme.backgroundColor
    radius: 8

    Layout.preferredWidth: Kirigami.Units.gridUnit * 12
    Layout.preferredHeight: Kirigami.Units.gridUnit * 12

    ListView {
        id: listView

        anchors.fill: parent
        model: plasmoid.nativeInterface.locationsInSystem()

        delegate: Delegates.RoundedItemDelegate {
            text: modelData

            onClicked: {
                selected();
                plasmoid.nativeInterface.setLocation(modelData);
            }
        }

        Label {
            anchors {
                bottom: parent.bottom
                bottomMargin: Kirigami.Units.gridUnit
            }

            color: Kirigami.Theme.disabledTextColor
            text: listView.count == 0 ? i18n("No location found on system, please add some in kweather") : i18n("Please select the location")
        }
    }
}
