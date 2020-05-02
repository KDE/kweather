import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtQuick.Shapes 1.12
import org.kde.kirigami 2.11 as Kirigami
import kweather 1.0

Kirigami.CardsListView {
    property WeatherLocation weatherLocation

    Layout.alignment: Qt.AlignHCenter
    anchors.fill: forecastView

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
                source: weatherLocation.currentForecast.weatherIcon
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
                    text: weatherLocation.currentForecast.weatherDescription
                }
                Label {
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 3
                    text: weatherLocation.currentForecast.maxTemp + "°" // TODO
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

            model: weatherLocation.hourListModel
            delegate: Rectangle {
                implicitWidth: 100
                implicitHeight: 170
                color: "transparent"

                property WeatherHour weather: weatherLocation.hourListModel.get(index)

                // actual hour display
                ColumnLayout {
                    id: hourElement
                    spacing: Kirigami.Units.smallSpacing

                    Label {
                        font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.15
                        text: weather.hour
                    }

                    Kirigami.Icon {
                        source: weather.weatherIcon
                        Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 4
                        Layout.minimumWidth: Layout.minimumHeight
                    }
                    Label {
                        text: weather.temperature + "°C"
                        font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
                    }
                    Label {
                        text: weather.weatherDescription
                    }

                    RowLayout {
                        Kirigami.Icon {
                            source: "raindrop"
                            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                            Layout.minimumWidth: Layout.minimumHeight
                        }
                        Label {
                            color: Kirigami.Theme.disabledTextColor
                            text: weather.precipitation
                        }
                    }
                }
            }
        }
    }

    model: weatherLocation.dayListModel

    // daily display
    delegate: Kirigami.Card {
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

                // weather icon and temperature
                ColumnLayout {
                    Kirigami.Icon {
                        Layout.alignment: Qt.AlignHCenter
                        source: weather.weatherIcon
                        Layout.maximumHeight: Kirigami.Units.iconSizes.medium
                        Layout.preferredWidth: height
                        Layout.preferredHeight: Kirigami.Units.iconSizes.medium
                    }
                    RowLayout {
                        Layout.alignment: Qt.AlignHCenter
                        Label {
                            id: highTemp
                            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.4
                            text: weather.maxTemp + "°"
                        }
                        Label {
                            anchors.baseline: highTemp.baseline
                            color: Kirigami.Theme.disabledTextColor
                            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1
                            text: weather.minTemp + "°"
                        }
                    }
                }

                // day and other info
                ColumnLayout {
                    Kirigami.Heading {
                        level: 2
                        text: weather.date.toLocaleDateString(Locale.shortFormat)
                    }
                    Kirigami.Separator {
                        Layout.fillWidth: true
                    }
                    Label {
                        text: weather.weatherDescription
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

