import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.12 as Kirigami

Kirigami.ScrollablePage {
    title: "Add city"

    property string searchQuery: ""

    ListView {
        id: addCityList
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
                onTextChanged: {
                    searchQuery = text
                    locationQueryModel.textChanged(text)
                }
                onEditingFinished: locationQueryModel.textChanged(text,0) // when return is pressed, query immediately
            }
        }

        // unable to connect message
        PlaceholderMessage {
            iconName: "network-disconnect"
            text: i18n("Unable to connect")
            visible: locationQueryModel.networkError
        }

        // default message (has not searched yet)
        PlaceholderMessage {
            iconName: "search"
            text: i18n("Search for a location")
            visible: !locationQueryModel.loading && addCityList.count == 0 && searchQuery == ""
        }

        // no results
        PlaceholderMessage {
            iconName: "search"
            text: i18n("No results")
            visible: !locationQueryModel.loading && addCityList.count == 0 && searchQuery != ""
        }

        // loading results indicator
        BusyIndicator {
            anchors.centerIn: parent
            running: locationQueryModel.loading
            Layout.minimumWidth: Kirigami.Units.iconSizes.huge
            Layout.minimumHeight: width
        }

        anchors.fill: parent
        model: locationQueryModel
        delegate: Kirigami.SwipeListItem {
            Label {
                text: model.name
            }
            onClicked: {
                locationQueryModel.addLocation(index);
                switchToPage(citiesPage);
            }
        }
    }
}


