import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtQuick.Shapes 1.12
import org.kde.kirigami 2.11 as Kirigami
import "components"

Rectangle {
    anchors.fill: parent
    color: "#455a64"
    gradient: Gradient {
        GradientStop { position: 0.0; color: "#455a64" }
        GradientStop { position: 1.0; color: "#263238" }
    }
    
    Stars {}
}
