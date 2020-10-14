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

Rectangle {
    color: "#85f1f7"
    Layout.fillWidth: true
    id: baseRect
    Shape {
        width: baseRect.width
        height: 200
        opacity: 0.8
        layer.enabled: true
        layer.samples: 4

        ShapePath {
            fillColor: "white"
            capStyle: ShapePath.FlatCap

            PathAngleArc {
                centerX: 65; centerY: 95
                radiusX: 45; radiusY: 45
                startAngle: 0
                sweepAngle: 360
            }
        }
    }
    Shape {
        width: baseRect.width
        height: 200
        opacity: 0.8
        layer.enabled: true
        layer.samples: 4
        ShapePath {
            fillColor: "white"
            capStyle: ShapePath.FlatCap

            PathAngleArc {
                centerX: 100; centerY: 20
                radiusX: 100; radiusY: 100
                startAngle: 0
                sweepAngle: 360
            }
        }
    }
    Shape {
        width: baseRect.width
        height: 200
        opacity: 0.9
        layer.enabled: true
        layer.samples: 4
        ShapePath {
            fillColor: "white"
            capStyle: ShapePath.FlatCap

            PathAngleArc {
                centerX: 150; centerY: 30
                radiusX: 90; radiusY: 90
                startAngle: 0
                sweepAngle: 360
            }
        }
    }
    Shape {
        width: baseRect.width
        height: 200
        opacity: 0.7
        layer.enabled: true
        layer.samples: 4
        ShapePath {
            fillColor: "white"
            capStyle: ShapePath.FlatCap

            PathAngleArc {
                centerX: 250; centerY: 60
                radiusX: 120; radiusY: 120
                startAngle: 0
                sweepAngle: 360
            }
        }
    }
    Shape {
        width: baseRect.width
        height: 200
        opacity: 0.8
        layer.enabled: true
        layer.samples: 4
        ShapePath {
            fillColor: "white"
            capStyle: ShapePath.FlatCap

            PathAngleArc {
                centerX: 400; centerY: 40
                radiusX: 50; radiusY: 50
                startAngle: 0
                sweepAngle: 360
            }
        }
    }
    Shape {
        width: baseRect.width
        height: 200
        opacity: 0.8
        layer.enabled: true
        layer.samples: 4
        ShapePath {
            fillColor: "white"
            capStyle: ShapePath.FlatCap

            PathAngleArc {
                centerX: 450; centerY: 70
                radiusX: 80; radiusY: 80
                startAngle: 0
                sweepAngle: 360
            }
        }
    }
    Shape {
        width: baseRect.width
        height: 200
        opacity: 0.9
        layer.enabled: true
        layer.samples: 4
        ShapePath {
            fillColor: "white"
            capStyle: ShapePath.FlatCap

            PathAngleArc {
                centerX: 350; centerY: 100
                radiusX: 90; radiusY: 90
                startAngle: 0
                sweepAngle: 360
            }
        }
    }
}
