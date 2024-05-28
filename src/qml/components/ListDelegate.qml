// Copyright 2022 Devin Lin <devin@kde.org>
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami

Control {
    id: root

    property bool showSeparator: false

    signal clicked
    signal rightClicked
    signal released
    signal pressAndHold

    property alias mouseArea: mouseArea

    leftPadding: Kirigami.Units.largeSpacing
    topPadding: Kirigami.Units.largeSpacing
    bottomPadding: Kirigami.Units.largeSpacing
    rightPadding: Kirigami.Units.largeSpacing

    hoverEnabled: true
    background: Rectangle {
        color: Qt.rgba(Kirigami.Theme.textColor.r, Kirigami.Theme.textColor.g, Kirigami.Theme.textColor.b, mouseArea.pressed ? 0.2 : (!Kirigami.Settings.tabletMode && hoverHandler.hovered) ? 0.1 : 0)

        HoverHandler {
            id: hoverHandler
        }

        Kirigami.Separator {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: root.leftPadding
            anchors.rightMargin: root.rightPadding
            visible: root.showSeparator
            opacity: 0.5
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onPressAndHold: root.pressAndHold()
        onReleased: root.released()
        onClicked: mouse => {
            if (mouse.button === Qt.RightButton) {
                root.rightClicked();
            } else if (mouse.button === Qt.LeftButton) {
                root.clicked();
            }
        }
    }
}
