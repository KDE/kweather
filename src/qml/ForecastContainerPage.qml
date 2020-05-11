import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtQuick.Shapes 1.12
import org.kde.kirigami 2.11 as Kirigami

Kirigami.ScrollablePage {
    id: page
    title: forecastView.count == 0 ? "Forecast" : weatherLocationListModel.get(forecastView.currentIndex).name
    PlaceholderMessage {
        iconName: "globe"
        text: i18n("No locations configured")
        visible: forecastView.count == 0
    }

    Timer {
        id: refreshRequestTimer
        interval: 3000
        onTriggered: page.refreshing = false
    }

    SwipeView {
        id: forecastView
        anchors.fill: parent

        Repeater {
            id: forecastViewRepeater
            model: weatherLocationListModel
            anchors.fill: parent

            CityForecast2 {
                weatherLocation: weatherLocationListModel.get(index)
            }
        }
    }
    PageIndicator {
        z: 10
        id: indicator

        count: forecastView.count
        currentIndex: forecastView.currentIndex

        anchors.bottom: forecastView.bottom
        anchors.horizontalCenter: forecastView.horizontalCenter
    }
}


