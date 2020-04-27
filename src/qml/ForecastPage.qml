import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtQuick.Shapes 1.12
import org.kde.kirigami 2.11 as Kirigami

Kirigami.ScrollablePage {
    title: "Forecast"
    /* c++ backend, not fully implemented yet
    property alias weatherForecast: forecastModel.weatherForecast

    WeatherForecastModel {
        id: forecastModel
        weatherForecastManager: WeatherForecastManager
    }
*/
    // example swipe down to refresh
    supportsRefreshing: true
    onRefreshingChanged: {
        if (refreshing) {
            refreshRequestTimer.running = true;
        } else {
            showPassiveNotification("Example refreshing completed")
        }
    }
    Timer {
        id: refreshRequestTimer
        interval: 1000
        onTriggered: forecastPage.refreshing = false
    }
    
    // view
    Kirigami.CardsListView {
        Layout.alignment: Qt.AlignHCenter
        anchors.fill: parent
        
        model: dayWeatherModel
        
        headerPositioning: ListView.InlineHeader
        header: ColumnLayout {
            id: topLayout
            
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Kirigami.Units.gridUnit * 2
            anchors.rightMargin: Kirigami.Units.gridUnit * 2

            spacing: Kirigami.Units.largeSpacing * 2
            
            // weather header
            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                Kirigami.Icon {
                    source: "weather-clouds"
                    Layout.preferredHeight: Kirigami.Theme.defaultFont.pointSize * 18
                    Layout.preferredWidth: Kirigami.Theme.defaultFont.pointSize * 18
                    Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 9
                    Layout.minimumWidth: Kirigami.Theme.defaultFont.pointSize * 9
                    smooth: true
                }
                ColumnLayout {
                    Label {
                        font.pointSize: Kirigami.Theme.defaultFont.pointSize * 2
                        font.weight: Font.Light
                        color: Kirigami.Theme.focusColor
                        text: "Toronto, ON"
                    }
                    Label {
                        font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
                        text: "Mostly Cloudy"
                    }
                    Label {
                        font.pointSize: Kirigami.Theme.defaultFont.pointSize * 4
                        text: "5°"
                    }
                }
            }
            
            // hourly
            ListView {
                orientation: ListView.Horizontal
                Layout.alignment: Qt.AlignHCenter
                
                implicitHeight: 170
                implicitWidth: parent.width
                spacing: Kirigami.Units.largeSpacing * 3
                clip: true

                model: hourWeatherModel
                // delegate: hourWeatherDelegate
                // see the bottom comment
                // this is a place holder for now
                delegate: Rectangle {
                    implicitWidth: 100
                    implicitHeight: 170
                    color: "transparent"

                    // actual hour display
                    ColumnLayout {
                        id: hourElement
                        spacing: Kirigami.Units.smallSpacing

                        Label {
                            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.15
                            text: time
                        }

                        Kirigami.Icon {
                            source: weatherIcon
                            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 4
                            Layout.minimumWidth: Layout.minimumHeight
                        }
                        Label {
                            text: temperature + "°C"
                            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
                        }
                        Label {
                            text: weather
                        }

                        RowLayout {
                            Kirigami.Icon {
                                source: "raindrop"
                                Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                                Layout.minimumWidth: Layout.minimumHeight
                            }
                            Label {
                                color: Kirigami.Theme.disabledTextColor
                                text: precipitation
                            }
                        }
                    }
                }
            }
        }

        // daily display
        delegate: Kirigami.Card {
            contentItem: Item {
                implicitWidth: delegateLayout.implicitWidth
                implicitHeight: delegateLayout.implicitHeight
                RowLayout {
                    id: delegateLayout
                    anchors {
                        left: parent.left
                        top: parent.top
                        right: parent.right
                        leftMargin: Kirigami.Units.largeSpacing
                        rightMargin: Kirigami.Units.largeSpacing
                    }

                    spacing: Kirigami.Units.largeSpacing * 2

                    // weather icon and temperature
                    ColumnLayout {
                        Kirigami.Icon {
                            Layout.alignment: Qt.AlignHCenter
                            source: weatherIcon
                            Layout.maximumHeight: Kirigami.Units.iconSizes.medium
                            Layout.preferredWidth: height
                            Layout.preferredHeight: Kirigami.Units.iconSizes.medium
                        }
                        RowLayout {
                            Layout.alignment: Qt.AlignHCenter
                            Label {
                                id: highTemp
                                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.4
                                text: highTemperature + "°"
                            }
                            Label {
                                anchors.baseline: highTemp.baseline
                                color: Kirigami.Theme.disabledTextColor
                                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1
                                text: lowTemperature + "°"
                            }
                        }
                    }

                    // day and other info
                    ColumnLayout {
                        Kirigami.Heading {
                            level: 2
                            text: model.day
                        }
                        Kirigami.Separator {
                            Layout.fillWidth: true
                        }
                        Label {
                            text: model.weather
                        }
                        // precipitation
                        RowLayout {
                            Kirigami.Icon {
                                source: "raindrop"
                                Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                                Layout.minimumWidth: Layout.minimumHeight
                            }
                            Label {
                                color: Kirigami.Theme.disabledTextColor
                                text: model.precipitation
                            }
                        }
                    }
                }
            }
        }
    }


    // hours
    ListModel {
        id: hourWeatherModel
        ListElement {
            weatherIcon: "weather-clouds"
            weather: "Mostly Cloudy"
            time: "5 pm"
            temperature: 4
            precipitation: "0 %"
        }
        ListElement {
            weatherIcon: "weather-clear"
            weather: "Sunny"
            time: "6 pm"
            temperature: 5
            precipitation: "0 %"
        }
        ListElement {
            weatherIcon: "weather-freezing-rain"
            weather: "Freezing Rain"
            time: "7 pm"
            temperature: -2
            precipitation: "80 %"
        }
        ListElement {
            weatherIcon: "weather-clear-night"
            weather: "Clear"
            time: "8 pm"
            temperature: -10
            precipitation: "5 %"
        }
        ListElement {
            weatherIcon: "weather-clouds-night"
            weather: "Mostly Cloudy"
            time: "9 pm"
            temperature: 4
            precipitation: "0 %"
        }
    }

    // days
    ListModel {
        id: dayWeatherModel
        ListElement {
            weatherIcon: "weather-clear"
            weather: "Sunny"
            day: "Wed 22"
            highTemperature: 5
            lowTemperature: -5
            precipitation: "0 %"
        }
        ListElement {
            weatherIcon: "weather-clouds"
            weather: "Mostly Cloudy"
            day: "Thu 23"
            highTemperature: 5
            lowTemperature: 2
            precipitation: "10 %"
        }
        ListElement {
            weatherIcon: "weather-clear"
            weather: "Sunny"
            day: "Fri 24"
            highTemperature: 12
            lowTemperature: 1
            precipitation: "0 %"
        }
        ListElement {
            weatherIcon: "weather-few-clouds"
            weather: "Mostly Sunny"
            day: "Sat 25"
            highTemperature: 11
            lowTemperature: 7
            precipitation: "0 %"
        }
        ListElement {
            weatherIcon: "weather-showers"
            weather: "Rain"
            day: "Sun 26"
            highTemperature: 10
            lowTemperature: -5
            precipitation: "90 %"
        }
        ListElement {
            weatherIcon: "weather-snow"
            weather: "Snow"
            day: "Mon 27"
            highTemperature: -5
            lowTemperature: -15
            precipitation: "100 %"
        }
        ListElement {
            weatherIcon: "weather-storm"
            weather: "Thunderstorm"
            day: "Tue 28"
            highTemperature: 35
            lowTemperature: 30
            precipitation: "70 %"
        }
    }
}
/*
Component {
id: hourWeatherDelegate
Kirigami.AbstractListItem{
    readonly property var fc: model.weatherForecast
    highlighted: false
    ColumnLayout {
        id: hourElement
        spacing: Kirigami.Units.smallSpacing

        Label {
            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.15
            text: model.localizedTime
        }

        Kirigami.Icon {
            source: fc.symbolIconName
            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 4
            Layout.minimumWidth: Layout.minimumHeight
        }
        Label {
            text: {if (fc.maximumTemperature === fc.minimumTemperature) {
                    return i18n("%1°C", fc.maximumTemperature);
                } else {
                    return i18n("%1°C / %2°C", fc.minimumTemperature, fc.maximumTemperature);
                }}
            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
        }
        Label {
            text: weather // todo implement weather description in weatherforecast class
        }

        RowLayout {
            Kirigami.Icon {
                source: "raindrop"
                Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                Layout.minimumWidth: Layout.minimumHeight
            }
            Label {
                color: Kirigami.Theme.disabledTextColor
                text: fc.precipitation
            }
        }
    }
}
}
*/

