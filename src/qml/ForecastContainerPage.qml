import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtQuick.Shapes 1.12
import org.kde.kirigami 2.11 as Kirigami

Kirigami.ScrollablePage {
    id: scrollablePage
    title: "Forecast"
    PlaceholderMessage {
        iconName: "globe"
        text: i18n("No locations configured")
        visible: forecastView.count == 0
    }

    SwipeView {
        id: forecastView
        anchors.fill: parent

        Repeater {
            model: weatherLocationListModel
            anchors.fill: parent

            CityForecast2 {
                weatherLocation: weatherLocationListModel.get(index)
            }
        }
    }
    PageIndicator {
        id: indicator

        count: forecastView.count
        currentIndex: forecastView.currentIndex

        anchors.top: forecastView.top
        anchors.horizontalCenter: parent.horizontalCenter
    }
}


