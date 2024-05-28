/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import org.kde.kirigami as Kirigami
import org.kde.kweather.backgrounds

ListView {
    id: root
    orientation: ListView.Horizontal
    implicitHeight: contentItem.childrenRect.height
    spacing: Kirigami.Units.largeSpacing * 3
    clip: true

    snapMode: ListView.SnapToItem

    property bool selectable: false

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
        visible: !Kirigami.Settings.isMobile && hoverMouseArea.hovered && (root.currentIndex > 0 || !root.atXBeginning)

        onClicked: {
            if (root.selectable) {
                root.decrementCurrentIndex();
            } else {
                animateMove.to -= root.contentItem.children[0].width + root.spacing;
                animateMove.start();
            }
        }
    }

    Button {
        icon.name: "arrow-right"
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        visible: !Kirigami.Settings.isMobile && hoverMouseArea.hovered && (root.currentIndex < root.count - 1 || !root.atXEnd)

        onClicked: {
            if (root.selectable) {
                root.incrementCurrentIndex();
            } else {
                animateMove.to += root.contentItem.children[0].width + root.spacing;
                animateMove.start();
            }
        }
    }

    NumberAnimation on contentX {
        id: animateMove
        to: root.contentX
        duration: Kirigami.Units.longDuration
        easing.type: Easing.InOutQuad
        onFinished: root.returnToBounds()
    }
}
