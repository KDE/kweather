import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.12 as Kirigami

Kirigami.ScrollablePage {
    title: i18n("Cities")
    
    actions.main: Kirigami.Action {
        iconName: "list-add"
    }
    
    Kirigami.CardsListView {
        id: citiesList
        model: citiesModel
        
        /*Kirigami.PlaceholderMessage {
            anchors.centerIn: parent
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: Kirigami.Units.largeSpacing
            visible: citiesList.count == 0
            text: "Add a city"
            icon.name: "globe"
        }
        */
        delegate: Kirigami.Card {
            
            actions: Kirigami.Action {
                iconName: "list-remove"
                text: "Remove"
                onTriggered: {
                    // todo delete city
                }
            }
            
            contentItem: Item {
                Column {
                    id: delegateLayout
                    
                    anchors {
                        left: parent.left
                        top: parent.top
                        right: parent.right
                    }
                    
                    Kirigami.Heading {
                        level: 3
                        text: model.name
                    }
                }
            }
        }
        
    }
    
    // mockup purposes
    ListModel {
        id: citiesModel
        
    }
}
