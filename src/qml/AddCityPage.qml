import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.11 as Kirigami

Kirigami.ScrollablePage {
    title: "Add city"

    ListView {
        header: Item {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Kirigami.Units.largeSpacing * 2
            anchors.rightMargin: Kirigami.Units.largeSpacing * 2
            height: search.height * 1.5

            TextField {
                id: search
                placeholderText: i18n("Search for a place...")
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: parent.right
                onTextChanged: locationQueryModel.setQuery(text)
            }
        }

        anchors.fill: parent
        ScrollBar.vertical: ScrollBar {}
        model: locationQueryModel
        delegate: Kirigami.SwipeListItem {
            Label {
                text: model.name
            }
        }
    }
}


