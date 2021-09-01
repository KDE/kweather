/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtCharts 2.3
import org.kde.kirigami 2.13 as Kirigami
import "backgrounds"

ListView {
    id: root
    orientation: ListView.Horizontal

    implicitHeight: contentItem.childrenRect.height
    spacing: Kirigami.Units.largeSpacing * 3
    clip: true

    snapMode: ListView.SnapToItem

    // detect mouse hover
    HoverHandler {
        id: hoverMouseArea
        acceptedDevices: PointerDevice.Mouse | PointerDevice.Stylus
    }

    // left right mouse controls
    Button {
        icon.name: "arrow-left"
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        visible: hoverMouseArea.hovered && root.currentIndex > 0

        onClicked: {
            root.decrementCurrentIndex()
        }
    }

    Button {
        icon.name: "arrow-right"
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        visible: hoverMouseArea.hovered && root.currentIndex < root.count - 1

        onClicked: {
            root.incrementCurrentIndex()
        }
    }
}
