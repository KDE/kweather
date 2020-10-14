import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtQuick.Shapes 1.12
import org.kde.kirigami 2.11 as Kirigami
import "components"

Rectangle {
    id: rootBackground
    anchors.fill: parent
    color: "#ffc107"

    property double unitSize: width / 100 // 100 is full width of screen
    property double innerRotation: 0
    property double outerRotation: 0
    property double outerOuterRotation: 0
    
    NumberAnimation on innerRotation {
        to: 360
        duration: 20000
        running: true
        onFinished: {
            innerRotation = 0
            restart();
        }
    }
    
    NumberAnimation on outerRotation {
        to: 360
        duration: 15000
        running: true
        onFinished: {
            outerRotation = 0
            restart();
        }
    }
    
    NumberAnimation on outerOuterRotation {
        to: 360
        duration: 12000
        running: true
        onFinished: {
            outerOuterRotation = 0
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
            rotation: outerOuterRotation + index * (90/6)
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
            rotation: outerRotation + index * (90/5)
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
            rotation: innerRotation + index * (90/3)
            x: rootBackground.width - unitSize * 10 - width / 2
            y: unitSize * 10 - height / 2
            antialiasing: true
        }
    }
}
