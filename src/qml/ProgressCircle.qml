import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtQuick.Shapes 1.12
import org.kde.kirigami 2.4 as Kirigami

Shape {
    id: circle

    property alias label: loader.sourceComponent
    property double progress // out of 1
    property int radius

    implicitWidth: radius*2+5
    implicitHeight: radius*2+5
//     anchors.horizontalCenter: parent.horizontalCenter

    layer.enabled: true
    layer.samples: 40

    Loader {
        anchors.centerIn: circle
        id: loader
    }

    // base circle
    ShapePath {
        id: timerCirclePath
        strokeColor: "lightgrey"
        fillColor: "transparent"
        strokeWidth: 4
        capStyle: ShapePath.FlatCap
        PathAngleArc {
            id: circleArc
            centerX: circle.width / 2; centerY: circle.height / 2;
            radiusX: radius; radiusY: radiusX
            startAngle: -180
            sweepAngle: 360
        }
    }

    // progress circle
    ShapePath {
        strokeColor: Kirigami.Theme.highlightColor
        fillColor: "transparent"
        strokeWidth: 4
        capStyle: ShapePath.FlatCap
        PathAngleArc {
            centerX: circleArc.centerX; centerY: circleArc.centerY
            radiusX: circleArc.radiusX; radiusY: circleArc.radiusY
            startAngle: -90
            sweepAngle: 360 * progress
        }
    }
}
