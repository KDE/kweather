/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtQuick.Shapes 1.12
import org.kde.kirigami 2.11 as Kirigami
import kweather 1.0

Kirigami.ScrollablePage {
    id: page
    verticalScrollBarPolicy: ScrollBar.AlwaysOff

    background: null // transparent, since there is a page behind

    property WeatherLocation weatherLocation
    property var currentDay: dailyListView.currentItem.weather

    property bool inView: false

    // swipe down to refresh
    supportsRefreshing: true
    onRefreshingChanged: {
        if (refreshing) {
            weatherLocation.update();
        } else {
            showPassiveNotification(i18n("Weather refreshed for %1", weatherLocation.name));
        }
    }
    Connections {
        target: weatherLocation
        function onStopLoadingIndicator() {
            page.refreshing = false;
        }
    }

    // all elements are in a column
    ColumnLayout {
        spacing: 0
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Kirigami.Icon {
                id: weatherIcon
                source: weatherLocation.hourListModel.currentForecast == null ? "weather-none-available" : weatherLocation.hourListModel.currentForecast.weatherIcon
                Layout.preferredHeight: width
                Layout.preferredWidth: page.width * 0.8 - headerText.width
                Layout.maximumHeight: Kirigami.Theme.defaultFont.pointSize * 15
                Layout.maximumWidth: Kirigami.Theme.defaultFont.pointSize * 15
                Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 5
                Layout.minimumWidth: Kirigami.Theme.defaultFont.pointSize * 5
                smooth: true
            }

            // weather header
            ColumnLayout {
                id: headerText
                Label {
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 3
                    font.weight: Font.Light
                    font.family: lightHeadingFont.name
                    text: weatherLocation.hourListModel.currentForecast == null ? "0" : weatherLocation.hourListModel.currentForecast.temperatureRounded
                }
                Label {
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
                    font.weight: Font.Bold
                    text: weatherLocation.hourListModel.currentForecast == null ? "Unknown" : weatherLocation.hourListModel.currentForecast.weatherDescription
                }
                Label {
                    color: Kirigami.Theme.disabledTextColor
                    Layout.topMargin: Kirigami.Units.largeSpacing
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 0.9
                    text: i18n("Updated at %1", weatherLocation.lastUpdated)
                }
            }
        }
        
        PageIndicator {
            visible: Kirigami.Settings.isMobile
            Layout.topMargin: Kirigami.Units.largeSpacing
            Layout.alignment: Qt.AlignHCenter
            opacity: forecastView.count > 1 ? 1 : 0
            count: forecastView.count
            currentIndex: forecastView.currentIndex
        }

        // daily view
        Label {
            text: i18n("Daily")
            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
            Layout.topMargin: Kirigami.Units.largeSpacing
        }
        Kirigami.Separator {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing * 2
        }
        ListView {
            orientation: ListView.Horizontal
            id: dailyListView

            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing * 2
            implicitHeight: Kirigami.Units.gridUnit * 8
            spacing: Kirigami.Units.largeSpacing
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

            model: weatherLocation.dayForecasts
            delegate: WeatherDayDelegate {
                weather: modelData
                textColor: Kirigami.Theme.textColor
            }

            // left right mouse controls
            HoverHandler {
                id: dayMouseArea
                acceptedDevices: PointerDevice.Mouse | PointerDevice.Stylus
            }
            Button {
                icon.name: "arrow-left"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                visible: (dayMouseArea.hovered || leftMouseArea.hovered) && dailyListView.currentIndex != 0
                onClicked: {
                    if (dailyListView.currentIndex > 0) {
                        dailyListView.currentIndex--;
                        weatherLocation.hourListModel.updateHourView(dailyListView.currentIndex);
                    }
                }
                HoverHandler {
                    id: leftMouseArea
                    acceptedDevices: PointerDevice.Mouse | PointerDevice.Stylus
                }
            }
            Button {
                icon.name: "arrow-right"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                visible: (dayMouseArea.hovered || rightMouseArea.hovered) && dailyListView.currentIndex != dailyListView.count - 1
                onClicked: {
                    if (dailyListView.currentIndex < dailyListView.count-1) {
                        dailyListView.currentIndex++;
                        weatherLocation.hourListModel.updateHourView(dailyListView.currentIndex);
                    }
                }
                HoverHandler {
                    id: rightMouseArea
                    acceptedDevices: PointerDevice.Mouse | PointerDevice.Stylus
                }
            }
        }

        // hourly view
        Label {
            Layout.topMargin: Kirigami.Units.largeSpacing * 2
            text: i18n("Hourly")
            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
        }
        Kirigami.Separator {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing * 2
        }
        ListView {
            id: weatherHourListView
            orientation: ListView.Horizontal

            Layout.topMargin: Kirigami.Units.largeSpacing * 2
            implicitHeight: Kirigami.Units.gridUnit * 10.5
            implicitWidth: parent.width
            spacing: Kirigami.Units.largeSpacing * 3
            clip: true

            snapMode: ListView.SnapToItem

            model: weatherLocation.hourListModel
            delegate: WeatherHourDelegate {
                weather: hourItem
                textColor: Kirigami.Theme.textColor
            }

            // left right mouse controls
            HoverHandler {
                id: hourMouseArea
                acceptedDevices: PointerDevice.Mouse | PointerDevice.Stylus
            }
            Button {
                id: control
                icon.name: "arrow-left"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                visible: hourMouseArea.hovered && weatherHourListView.contentX != 0
                SmoothedAnimation {
                    target: weatherHourListView
                    property: "contentX"
                    running: control.pressed
                    to: 0
                    velocity: 500
                    maximumEasingTime: 0
                }
            }
            Button {
                id: control2
                icon.name: "arrow-right"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                visible: hourMouseArea.hovered && weatherHourListView.contentX != weatherHourListView.contentWidth - weatherHourListView.width
                SmoothedAnimation {
                    target: weatherHourListView
                    property: "contentX"
                    running: control2.pressed
                    to: weatherHourListView.contentWidth - weatherHourListView.width
                    velocity: 500
                    maximumEasingTime: 0
                }
            }

        }

        // bottom card (extra info for selected day)
        Kirigami.Card {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing * 2

            contentItem: Item {
                implicitHeight: column.height
                Column {
                    id: column
                    spacing: Kirigami.Units.largeSpacing

                    // precipitation
                    RowLayout {
                        spacing: Kirigami.Units.largeSpacing
                        Kirigami.Icon {
                            source: "raindrop"
                            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                            Layout.minimumWidth: Layout.minimumHeight * 1.5
                        }
                        Column {
                            spacing: Kirigami.Units.smallSpacing
                            Label {
                                font.weight: Font.Bold
                                text: i18n("Precipitation")
                            }
                            Label {
                                text: currentDay == null ? "" : currentDay.precipitation.toFixed(1) + "mm"
                            }
                        }
                    }

                    Kirigami.Separator {
                        Layout.fillWidth: true
                    }

                    // Humidity
                    RowLayout {
                        spacing: Kirigami.Units.largeSpacing
                        Kirigami.Icon {
                            source: "compass"
                            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                            Layout.minimumWidth: Layout.minimumHeight * 1.5
                        }
                        Column {
                            spacing: Kirigami.Units.smallSpacing
                            Label {
                                font.weight: Font.Bold
                                text: i18n("Humidity")
                            }
                            Label {
                                text: currentDay == null ? "" : i18n("%1%", currentDay.humidity.toFixed(1))
                            }
                        }
                    }

                    Kirigami.Separator {
                        Layout.fillWidth: true
                    }

                    // Atmospheric pressure
                    RowLayout {
                        spacing: Kirigami.Units.largeSpacing
                        Kirigami.Icon {
                            source: "speedometer"
                            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                            Layout.minimumWidth: Layout.minimumHeight * 1.5
                        }
                        Column {
                            spacing: Kirigami.Units.smallSpacing
                            Label {
                                font.weight: Font.Bold
                                text: i18n("Pressure")
                            }
                            Label {
                                text: currentDay == null ? "" : i18n("%1hPa", currentDay.pressure.toFixed(1))
                            }
                        }
                    }

                    Kirigami.Separator {
                        Layout.fillWidth: true
                    }

                    // UV Index
                    RowLayout {
                        spacing: Kirigami.Units.largeSpacing
                        Kirigami.Icon {
                            source: "compass"
                            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                            Layout.minimumWidth: Layout.minimumHeight * 1.5
                        }
                        Column {
                            spacing: Kirigami.Units.smallSpacing
                            Label {
                                font.weight: Font.Bold
                                text: i18n("UV index")
                            }
                            Label {
                                text: currentDay == null ? "" : currentDay.uvIndex.toFixed(1)
                            }
                        }
                    }
                }
            }
        }

        // sunrise/sunset card
        Kirigami.Card {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing * 2

            contentItem: Item {
                implicitHeight: sunsetColumn.height

                Column {
                    id: sunsetColumn
                    spacing: Kirigami.Units.largeSpacing

                    // Sunrise
                    RowLayout {
                        spacing: Kirigami.Units.largeSpacing
                        Kirigami.Icon {
                            source: "go-up"
                            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                            Layout.minimumWidth: Layout.minimumHeight * 1.5
                        }
                        Column {
                            spacing: Kirigami.Units.smallSpacing
                            Label {
                                font.weight: Font.Bold
                                text: i18n("Sunrise")
                            }
                            Label {
                                text: Qt.formatTime(currentDay.sunrise.sunRise, Locale.ShortFormat)
                            }
                        }
                    }

                    Kirigami.Separator {
                        Layout.fillWidth: true
                    }

                    // Sunset
                    RowLayout {
                        spacing: Kirigami.Units.largeSpacing
                        Kirigami.Icon {
                            source: "go-down"
                            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                            Layout.minimumWidth: Layout.minimumHeight * 1.5
                        }
                        Column {
                            spacing: Kirigami.Units.smallSpacing
                            Label {
                                font.weight: Font.Bold
                                text: i18n("Sunset")
                            }
                            Label {
                                text: Qt.formatTime(currentDay.sunrise.sunSet, Locale.ShortFormat)
                            }
                        }
                    }

                    Kirigami.Separator {
                        Layout.fillWidth: true
                    }

                    // Moon phase
                    RowLayout {
                        spacing: Kirigami.Units.largeSpacing
                        Kirigami.Icon {
                            source: "weather-clear-night"
                            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                            Layout.minimumWidth: Layout.minimumHeight * 1.5
                        }
                        Column {
                            spacing: Kirigami.Units.smallSpacing
                            Label {
                                font.weight: Font.Bold
                                text: i18n("Moon Phase")
                            }
                            Label {
                                text: currentDay.sunrise.moonPhaseString
                            }
                        }
                    }
                }
            }
        }
    }
}

