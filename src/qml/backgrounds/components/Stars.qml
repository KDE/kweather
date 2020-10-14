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

Item {
    id: rootItem
    anchors.fill: parent
    property double starRadius: Math.max(1, Math.min(width, height) / 800)
    
    Repeater {
        model: 30
        
        Shape {
            id: shape
            anchors.fill: rootItem
            layer.enabled: true
            layer.samples: 4
            opacity: 1
            
            vendorExtensionsEnabled: true
            asynchronous: true
            
            NumberAnimation on opacity {
                to: 0
                duration: Math.max(1000, 10000 * Math.random())
                easing.type: Easing.InOutQuad
                onFinished: {
                    if (shape.opacity === 0) {
                        to = 1;
                    } else {
                        to = 0;
                    }
                    restart();
                }
            }
            
            
            ShapePath {
                id: shapePath
                capStyle: ShapePath.FlatCap
                strokeColor: "transparent"
                fillColor: "#fff9c4"

                PathAngleArc {
                    id: angleArc
                    centerX: Math.random() * rootItem.width; centerY: Math.random() * rootItem.height
                    radiusX: starRadius; radiusY: radiusX
                    startAngle: 0
                    sweepAngle: 360
                }
            }
        }
    }
}
