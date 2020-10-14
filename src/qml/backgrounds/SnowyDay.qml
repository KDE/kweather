import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtQuick.Shapes 1.12
import org.kde.kirigami 2.11 as Kirigami
import "components"

Rectangle {
    gradient: Gradient {
        GradientStop { color: "#3daee2"; position: 0.0 }
        GradientStop { color: "#3daee2"; position: 1.0 }
    }
    
    Snow {
        z: 2
        backGroundColor: "transparent"
    }
    Cloudy {
        cloudColor: "white"
    }
}

