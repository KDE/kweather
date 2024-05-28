/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2021 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes
import org.kde.kirigami as Kirigami

Rectangle {
    id: rootBackground
    color: "transparent"
    anchors.fill: parent

    property double unitSize: width / 100 // 100 is full width of screen
    property double innerRotation: 0
    property double outerRotation: 0
    property double outerOuterRotation: 0

    NumberAnimation on innerRotation {
        to: 360
        duration: 20000
        running: true
        onFinished: {
            innerRotation = 0;
            restart();
        }
    }

    NumberAnimation on outerRotation {
        to: 360
        duration: 15000
        running: true
        onFinished: {
            outerRotation = 0;
            restart();
        }
    }

    NumberAnimation on outerOuterRotation {
        to: 360
        duration: 12000
        running: true
        onFinished: {
            outerOuterRotation = 0;
            restart();
        }
    }
    // outer outer sun
    Repeater {
        model: 6
        Rectangle {
            width: unitSize * 120
            height: width
            color: "#ffa000"
            opacity: 0.4
            border.width: 0
            rotation: outerOuterRotation + index * (90 / 6)
            x: rootBackground.width - unitSize * 10 - width / 2
            y: unitSize * 10 - height / 2
            antialiasing: true
        }
    }

    // outer sun
    Repeater {
        model: 5
        Rectangle {
            width: unitSize * 60
            height: width
            color: "#ff8f00"
            opacity: 0.5
            border.width: 0
            rotation: outerRotation + index * (90 / 5)
            x: rootBackground.width - unitSize * 10 - width / 2
            y: unitSize * 10 - height / 2
            antialiasing: true
        }
    }

    // inner sun
    Repeater {
        model: 3
        Rectangle {
            width: unitSize * 30
            height: width
            color: "#ff6f00"
            opacity: 0.5
            border.width: 0
            rotation: innerRotation + index * (90 / 3)
            x: rootBackground.width - unitSize * 10 - width / 2
            y: unitSize * 10 - height / 2
            antialiasing: true
        }
    }
}
