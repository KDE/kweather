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
    property bool inView: false
    
    property double opacityModifier: 0
    
    NumberAnimation on opacityModifier {
        running: inView
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
        
        Shape {
            id: shape
            anchors.fill: rootItem
            layer.enabled: true
            layer.samples: 4
            
            property double starModifier: {
                let num = 5000 * Math.random()
                if (num < 1000) {
                    return 1000;
                } else {
                    return num;
                }
            }
            
            opacity: {
                let remainder = rootItem.opacityModifier % (2 * starModifier);
                if (remainder > starModifier) { // opacity is decreasing
                    return (starModifier - (remainder % starModifier)) / starModifier;
                } else { // opacity is increasing
                    return remainder / starModifier;
                }
            }
            
            vendorExtensionsEnabled: true
            asynchronous: true
            
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
