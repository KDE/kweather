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
    id: rootItem
    anchors.fill: parent
    property double starRadius: 1

    property double opacityModifier: 0

    NumberAnimation on opacityModifier {
        running: true
        to: 10000
        duration: Math.max(10000)
        onFinished: {
            if (rootItem.opacityModifier === 0) {
                to = 10000;
            } else {
                to = 0;
            }
            restart();
        }
    }

    Repeater {
        model: 30

        Item {
            id: shape
            anchors.fill: rootItem

            property double starModifier: {
                let num = 5000 * Math.random();
                if (num < 1000) {
                    return 1000;
                } else {
                    return num;
                }
            }

            opacity: {
                let remainder = rootItem.opacityModifier % (2 * starModifier);
                if (remainder > starModifier) {
                    // opacity is decreasing
                    return (starModifier - (remainder % starModifier)) / starModifier;
                } else {
                    // opacity is increasing
                    return remainder / starModifier;
                }
            }

            Rectangle {
                color: "#fff9c4"
                height: starRadius * 2
                width: height
                radius: width / 2
                x: Math.random() * rootItem.width
                y: Math.random() * rootItem.height
            }
        }
    }
}
