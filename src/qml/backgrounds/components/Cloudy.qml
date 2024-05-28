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
    id: baseRect
    opacity: 0.65
    anchors.fill: parent
    property color cloudColor: "white"
    property double unitSize: width / 100 // 100 is full width of screen
    property double heightUnitSize: height / 100

    Cloud {
        opacity: 0.2
        color: baseRect.cloudColor
        maxRadiusChange: unitSize * 3
        maxCoordChange: unitSize * 6
        minRadius: unitSize * 40
        minX: unitSize * 10
        minY: heightUnitSize * -3
    }
    Cloud {
        opacity: 0.3
        color: baseRect.cloudColor
        maxRadiusChange: unitSize * 2
        maxCoordChange: unitSize * 5
        minRadius: unitSize * 40
        minX: unitSize * 40
        minY: heightUnitSize * 20
    }
    Cloud {
        opacity: 0.2
        color: baseRect.cloudColor
        maxRadiusChange: unitSize * 3
        maxCoordChange: unitSize * 2
        minRadius: unitSize * 23
        minX: unitSize * 85
        minY: heightUnitSize * 7
    }
    Cloud {
        opacity: 0.2
        color: baseRect.cloudColor
        maxRadiusChange: unitSize * 2
        maxCoordChange: unitSize * 6
        minRadius: unitSize * 40
        minX: unitSize * 110
        minY: heightUnitSize * -6
    }
    Cloud {
        opacity: 0.5
        color: baseRect.cloudColor
        maxRadiusChange: unitSize * 3
        maxCoordChange: unitSize * 2
        minRadius: unitSize * 32
        minX: unitSize * 32
        minY: heightUnitSize * 6
    }
    Cloud {
        opacity: 0.6
        color: baseRect.cloudColor
        maxRadiusChange: unitSize * 3
        maxCoordChange: unitSize * 6
        minRadius: unitSize * 24
        minX: unitSize * 48
        minY: heightUnitSize * 10
    }
    Cloud {
        opacity: 0.5
        color: baseRect.cloudColor
        maxRadiusChange: unitSize * 5
        maxCoordChange: unitSize * 2
        minRadius: unitSize * 27
        minX: unitSize * 90
        minY: heightUnitSize * 17
    }
}
