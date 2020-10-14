/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtQuick.Shapes 1.12
import org.kde.kirigami 2.11 as Kirigami

Shape {
    id: rootShape
    property double maxRadiusChange
    property double maxCoordChange
    property double minRadius
    property double minX
    property double minY
    property double radius: minRadius
    property double centerX: minX
    property double centerY: minY
    property alias color: shapePath.fillColor
    
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
    
    width: baseRect.width
    height: baseRect.height
    layer.enabled: true
    layer.samples: 4
    ShapePath {
        id: shapePath
        capStyle: ShapePath.FlatCap
        strokeColor: "transparent"

        PathAngleArc {
            id: angleArc
            centerX: rootShape.centerX; centerY: rootShape.centerY
            radiusX: rootShape.radius; radiusY: radiusX
            startAngle: 0
            sweepAngle: 360
        }
    }
}
