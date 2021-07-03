import QtQuick 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.4
import org.kde.plasma.plasmoid 2.0
import org.kde.kirigami 2.11 as Kirigami
Rectangle {
    signal selected()
    id: container
    Layout.preferredWidth: Kirigami.Units.gridUnit * 12
    Layout.preferredHeight: Kirigami.Units.gridUnit * 12
    color: Kirigami.Theme.backgroundColor
    radius: 8

    ListView {
        id: listView
        anchors.fill: parent
        model: plasmoid.nativeInterface.locationsInSystem()
        delegate: Kirigami.BasicListItem {
            text: modelData
            onClicked: {
                selected()
                plasmoid.nativeInterface.setLocation(modelData)
            }
        }
        Label {
            anchors.bottom: parent.bottom
            anchors.bottomMargin: Kirigami.Units.gridUnit
            color: Kirigami.Theme.disabledTextColor
            text: listView.count == 0 ? i18n("No location found on system, please add some in kweather") : i18n("please select the location")
        }
    }
}
