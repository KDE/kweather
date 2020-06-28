/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
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
    property WeatherDay currentDay: weatherLocation.dayListModel.get(dailyListView.currentIndex)

    // swipe down to refresh
    supportsRefreshing: true
    onRefreshingChanged: {
        if (refreshing) {
            weatherLocation.updateBackend();
        } else {
            showPassiveNotification(i18n("Weather refreshed for %1", weatherLocation.name));
        }
    }
    Connections {
        target: weatherLocation
        onStopLoadingIndicator: {page.refreshing = false}
    }

    // all elements are in a column
    ColumnLayout {
        spacing: Kirigami.Units.largeSpacing * 2

        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Kirigami.Icon {
                id: weatherIcon
                source: weatherLocation.currentWeather == null ? "weather-none-available" : weatherLocation.currentWeather.weatherIcon
                Layout.preferredHeight: Kirigami.Theme.defaultFont.pointSize * 15
                Layout.preferredWidth: Kirigami.Theme.defaultFont.pointSize * 15
                Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 15
                Layout.minimumWidth: Kirigami.Theme.defaultFont.pointSize * 15
                smooth: true
                Layout.alignment: Qt.AlignHCenter
            }

            // weather header
            ColumnLayout {
                Label {
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 3
                    font.weight: Font.Light
                    text: weatherLocation.currentWeather == null ? "0" : weatherLocation.currentWeather.temperatureRounded
                }
                Label {
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
                    font.weight: Font.Bold
                    text: weatherLocation.currentWeather == null ? "Unknown" : weatherLocation.currentWeather.weatherDescription
                }
                Label {
                    color: Kirigami.Theme.disabledTextColor
                    Layout.topMargin: Kirigami.Units.largeSpacing
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 0.9
                    text: i18n("Updated at %1", weatherLocation.lastUpdated)
                }
            }
        }

        Kirigami.Separator {}

        // daily view
        Label {
            text: i18n("Daily")
            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
        }
        Kirigami.Separator {
            Layout.fillWidth: true
        }
        ListView {
            orientation: ListView.Horizontal
            id: dailyListView

            Layout.fillWidth: true
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

            model: weatherLocation.dayListModel
            delegate: WeatherDayDelegate {
                weather: weatherLocation == null ? null : weatherLocation.dayListModel.get(index)
            }

            // left right mouse controls
            MouseArea {
                id: dayMouseArea
                anchors.fill: parent
                hoverEnabled: true
                z: -1
            }
            Button {
                icon.name: "arrow-left"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                visible: (dayMouseArea.containsMouse || leftMouseArea.containsMouse) && dailyListView.currentIndex != 0
                onClicked: {
                    if (dailyListView.currentIndex > 0) {
                        dailyListView.currentIndex--;
                        weatherLocation.hourListModel.updateHourView(dailyListView.currentIndex);
                    }
                }
                MouseArea {
                    id: leftMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: parent.clicked()
                    onPressed: {
                        parent.checkable = true;
                        parent.checked = true;
                    }
                    onReleased: {
                        parent.checkable = false;
                        parent.checked = false;
                    }
                }
            }
            Button {
                icon.name: "arrow-right"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                visible: (dayMouseArea.containsMouse || rightMouseArea.containsMouse) && dailyListView.currentIndex != dailyListView.count - 1
                onClicked: {
                    if (dailyListView.currentIndex < dailyListView.count-1) {
                        dailyListView.currentIndex++;
                        weatherLocation.hourListModel.updateHourView(dailyListView.currentIndex);
                    }
                }
                MouseArea {
                    id: rightMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: parent.clicked()
                    onPressed: {
                        parent.checkable = true;
                        parent.checked = true;
                    }
                    onReleased: {
                        parent.checkable = false;
                        parent.checked = false;
                    }
                }
            }

        }

        // hourly view
        Label {
            text: i18n("Hourly")
            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
        }
        Kirigami.Separator {
            Layout.fillWidth: true
        }
        ListView {
            id: weatherHourListView
            orientation: ListView.Horizontal

            implicitHeight: Kirigami.Units.gridUnit * 9
            implicitWidth: parent.width
            spacing: Kirigami.Units.largeSpacing * 3
            clip: true

            snapMode: ListView.SnapToItem

            model: weatherLocation.hourListModel
            delegate: WeatherHourDelegate {
                weather: weatherLocation == null ? null : weatherLocation.hourListModel.get(index)
            }

            // left right mouse controls
            MouseArea {
                id: hourMouseArea
                anchors.fill: parent
                hoverEnabled: true
            }
            Button {
                id: control
                icon.name: "arrow-left"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                visible: hourMouseArea.containsMouse && weatherHourListView.contentX != 0
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
                visible: hourMouseArea.containsMouse && weatherHourListView.contentX != weatherHourListView.contentWidth - weatherHourListView.width
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

        Kirigami.Separator {}

        // bottom card (extra info for selected day)
        Kirigami.Card {
            Layout.fillWidth: true

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
                            source: "compass"
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

            contentItem: Item {
                implicitHeight: sunsetColumn.height

                Column {
                    id: sunsetColumn
                    spacing: Kirigami.Units.largeSpacing

                    // Sunrise
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
                                text: i18n("Sunrise")
                            }
                            Label {
                                text: currentDay == null ? "" : currentDay.sunrise
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
                            source: "compass"
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
                                text: currentDay == null ? "" : currentDay.sunset
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
                                text: currentDay == null ? "" : currentDay.moonPhase
                            }
                        }
                    }
                }
            }
        }
    }
}

