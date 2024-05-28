/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes
import org.kde.kirigami as Kirigami

Item {
    id: rootShape
    anchors.fill: parent

    property double maxRadiusChange
    property double maxCoordChange
    property double minRadius
    property double minX
    property double minY
    property double radius: minRadius
    property double centerX: minX
    property double centerY: minY
    property color color

    NumberAnimation on radius {
        duration: 4000
        running: true
        easing.type: Easing.InOutQuad
        onFinished: {
            to = minRadius + Math.random() * maxRadiusChange;
            restart();
        }
    }

    NumberAnimation on centerX {
        duration: 2600
        running: true
        easing.type: Easing.InOutQuad
        onFinished: {
            to = minX + Math.random() * maxCoordChange;
            restart();
        }
    }

    NumberAnimation on centerY {
        duration: 3400
        running: true
        easing.type: Easing.InOutQuad
        onFinished: {
            to = minY + Math.random() * maxCoordChange;
            restart();
        }
    }

    Rectangle {
        radius: width / 2
        color: rootShape.color
        width: rootShape.radius * 2
        height: width
        x: centerX - width / 2
        y: centerY - width / 2
    }
}
