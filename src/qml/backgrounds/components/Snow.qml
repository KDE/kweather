import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtQuick.Shapes 1.12
import org.kde.kirigami 2.11 as Kirigami

Rectangle {
    property color backGroundColor: "#3daee2"
    color: backGroundColor
    anchors.fill: parent
    id: baseRect

    Item {
        anchors.top: parent.top
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
                    centerX: 400; centerY: 40
                    radiusX: 50; radiusY: 50
                    startAngle: 0
                    sweepAngle: 360
                }
                PathAngleArc {
                    centerX: 200; centerY: 180
                    radiusX: 25; radiusY: 25
                    startAngle: 0
                    sweepAngle: 360
                }
                PathAngleArc {
                    centerX: 380; centerY: 120
                    radiusX: 15; radiusY: 15
                    startAngle: 0
                    sweepAngle: 360
                }
                PathAngleArc {
                    centerX: 300; centerY: 150
                    radiusX: 25; radiusY: 25
                    startAngle: 0
                    sweepAngle: 360
                }
                PathAngleArc {
                    centerX: 180; centerY: 120
                    radiusX: 35; radiusY: 35
                    startAngle: 0
                    sweepAngle: 360
                }
                PathAngleArc {
                    centerX: 80; centerY: 90
                    radiusX: 15; radiusY: 15
                    startAngle: 0
                    sweepAngle: 360
                }
            }
        }
    }
}
