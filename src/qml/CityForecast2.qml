import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtQuick.Shapes 1.12
import org.kde.kirigami 2.11 as Kirigami
import kweather 1.0

ListView {
    property WeatherLocation weatherLocation

    Layout.alignment: Qt.AlignHCenter
    anchors.fill: forecastView

    headerPositioning: ListView.InlineHeader
    header: ColumnLayout {
        id: topLayout

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: Kirigami.Units.gridUnit
        anchors.leftMargin: Kirigami.Units.gridUnit

        spacing: Kirigami.Units.largeSpacing * 2

        // weather header
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Kirigami.Icon {
                source: weatherLocation.currentWeather.neutralWeatherIcon
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
                    text: weatherLocation.name
                }
                Label {
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
                    text: weatherLocation.currentWeather.weatherDescription
                }
                Label {
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 3
                    text: Math.round(weatherLocation.currentWeather.temperature) + "°" // TODO
                }
            }
        }

        Label {
            font: Kirigami.Theme.smallFont
            text: qsTr("Updated at ") + weatherLocation.lastUpdated
            Layout.alignment: Qt.AlignHCenter
        }

        Label {
            text: i18n("Daily")
            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
        }
        Kirigami.Separator {
            Layout.fillWidth: true
        }

        // daily
        ListView {
            orientation: ListView.Horizontal
            id: dailyListView

            anchors.left: parent.left
            anchors.right: parent.right
            implicitHeight: Kirigami.Units.gridUnit * 8
            spacing: Kirigami.Units.largeSpacing * 2
            clip: true

            snapMode: ListView.SnapToItem

            highlightMoveDuration: 400
            highlightMoveVelocity: -1
            highlight: Rectangle {
                color: Kirigami.Theme.focusColor
                border {
                    color: Kirigami.Theme.focusColor
                    width: 1
                }
                radius: 4
                opacity: 0.3
                focus: true
            }

            currentIndex: 0

            model: weatherLocation.dayListModel
            delegate: Rectangle {
                implicitWidth: Kirigami.Units.gridUnit * 6
                implicitHeight: Kirigami.Units.gridUnit * 8
                color: "transparent"
                anchors.fill: dayElement

                property WeatherDay weather: weatherLocation.dayListModel.get(index)

                MouseArea {
                    z: 1
                    anchors.fill: parent
                    onClicked: {
                        dailyListView.currentIndex = index
                        weatherLocation.hourListModel.updateUi(index) // change hour view
                    }
                }

                // actual day display
                ColumnLayout {
                    id: dayElement
                    anchors.fill: parent
                    anchors.leftMargin: Kirigami.Units.largeSpacing
                    anchors.topMargin: Kirigami.Units.largeSpacing
                    anchors.rightMargin: Kirigami.Units.largeSpacing
                    anchors.bottomMargin: Kirigami.Units.largeSpacing
                    spacing: Kirigami.Units.smallSpacing

                    Label {
                        font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1
                        text: weather != null ? weather.date.toLocaleString(Qt.locale(), "ddd d").replace(".", "") : ""
                    }

                    Kirigami.Icon {
                        source: weather != null ? weather.weatherIcon : ""
                        Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 3.5
                        Layout.minimumWidth: Layout.minimumHeight
                    }
                    RowLayout {
                        Label {
                            id: highTemp
                            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
                            text: weather != null ? weather.maxTemp + "°" : ""
                        }
                        Label {
                            anchors.baseline: highTemp.baseline
                            color: Kirigami.Theme.disabledTextColor
                            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1
                            text: weather != null ? weather.minTemp + "°" : ""
                        }
                    }
                    Label {
                        text: weather != null ? weather.weatherDescription : ""
                    }
                }
            }
        }

        Label {
            text: i18n("Hourly")
            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
        }
        Kirigami.Separator {
            Layout.fillWidth: true
        }

        // hourly
        ListView {
            orientation: ListView.Horizontal

            implicitHeight: Kirigami.Units.gridUnit * 9
            implicitWidth: parent.width
            spacing: Kirigami.Units.largeSpacing * 3
            clip: true

            snapMode: ListView.SnapToItem

            model: weatherLocation.hourListModel
            delegate: Rectangle {
                implicitWidth: Kirigami.Units.gridUnit * 5
                implicitHeight: Kirigami.Units.gridUnit * 8
                color: "transparent"
                property WeatherHour weather: weatherLocation.hourListModel.get(index)

                // actual hour display
                ColumnLayout {
                    id: hourElement
                    anchors.fill: parent
                    anchors.leftMargin: Kirigami.Units.largeSpacing
                    anchors.topMargin: Kirigami.Units.largeSpacing
                    anchors.rightMargin: Kirigami.Units.largeSpacing
                    anchors.bottomMargin: Kirigami.Units.largeSpacing
                    spacing: Kirigami.Units.smallSpacing

                    Kirigami.Icon {
                        source: weather != null ? weather.weatherIcon : ""
                        Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 3.5
                        Layout.minimumWidth: Layout.minimumHeight
                    }
                    Label {
                        text: weather != null ? weather.temperature + "°" : ""
                        font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
                    }
                    Label {
                        text: weather != null ? weather.weatherDescription : ""
                    }

                    RowLayout {
                        Kirigami.Icon {
                            source: "raindrop"
                            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                            Layout.minimumWidth: Layout.minimumHeight
                        }
                        Label {
                            color: Kirigami.Theme.disabledTextColor
                            text: weather != null ? weather.precipitation.toFixed(1) + "mm" : ""
                        }
                    }

                    Label {
                        font.weight: Font.Bold
                        font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1
                        text: weather != null ? weather.date.toLocaleString(Qt.locale(), "h ap").replace(".", "").replace(".", "") : ""
                    }
                }
            }
        }

        // daily display
        Kirigami.Card {
            anchors.left: parent.left
            anchors.right: parent.right

            property WeatherDay weather: weatherLocation.dayListModel.get(index)

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

                    Kirigami.Icon {
                        Layout.alignment: Qt.AlignHCenter
                        source: "raindrop"
                        Layout.maximumHeight: Kirigami.Units.iconSizes.medium
                        Layout.preferredWidth: height
                        Layout.preferredHeight: Kirigami.Units.iconSizes.medium
                    }


                    // day and other info
                    ColumnLayout {
                        Kirigami.Heading {
                            level: 2
                            text: i18n("Probability of Precipitation")
                        }
                        Kirigami.Separator {
                            Layout.fillWidth: true
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
                                text: "50 %" // TODO
                            }
                        }
                    }
                }
            }
        }
    }
}

