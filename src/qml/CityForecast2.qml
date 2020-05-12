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
    Layout.fillWidth: true
    verticalScrollBarPolicy: ScrollBar.AlwaysOff

    property WeatherLocation weatherLocation

    // swipe down to refresh
    supportsRefreshing: true
    onRefreshingChanged: {
        if (refreshing) {
            weatherLocation.updateBackend();
        } else {
            showPassiveNotification("Weather refreshed for " + weatherLocation.name);
        }
    }
    Connections {
        target: weatherLocation
        onStopLoadingIndicator: {page.refreshing = false}
    }

    // weather header
    ColumnLayout {
        Layout.fillWidth: true
        anchors.leftMargin: 1
        anchors.rightMargin: 1
        spacing: Kirigami.Units.largeSpacing * 2
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Kirigami.Icon {
                id: weatherIcon
                source: weatherLocation.currentWeather.weatherIcon
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
                    text: weatherLocation.currentWeather.temperature
                }
            }
        }

        Label {
            font: Kirigami.Theme.smallFont
            text: qsTr("Updated at ") + weatherLocation.lastUpdated
            Layout.alignment: Qt.AlignHCenter
        }

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
            orientation: ListView.Horizontal

            implicitHeight: Kirigami.Units.gridUnit * 9
            implicitWidth: parent.width
            spacing: Kirigami.Units.largeSpacing * 3

            snapMode: ListView.SnapToItem

            model: weatherLocation.hourListModel
            delegate: WeatherHourDelegate {
                weather: weatherLocation == null ? null : weatherLocation.hourListModel.get(index)
            }
        }

        // bottom card (extra info for selected day)
        Kirigami.Card {
            Layout.fillWidth: true

            contentItem: Item {
                implicitHeight: Kirigami.Units.gridUnit * 11
                Column {
                    spacing: Kirigami.Units.largeSpacing

                    // precipitation
                    RowLayout {
                        Kirigami.Icon {
                            source: "raindrop"
                            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                            Layout.minimumWidth: Layout.minimumHeight
                        }
                        Column {
                            Label {
                                font.weight: Font.Bold
                                text: "Precipitation"
                            }
                            Label {
                                text: weatherLocation.dayListModel.get(dailyListView.currentIndex).precipitation.toFixed(1) + "mm"
                            }
                        }
                    }

                    Kirigami.Separator {
                        Layout.fillWidth: true
                    }

                    // Humidity
                    RowLayout {
                        Kirigami.Icon {
                            source: "compass"
                            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                            Layout.minimumWidth: Layout.minimumHeight
                        }
                        Column {
                            Label {
                                font.weight: Font.Bold
                                text: "Humidity"
                            }
                            Label {
                                text: weatherLocation.dayListModel.get(dailyListView.currentIndex).humidity.toFixed(1) + "%"
                            }
                        }
                    }

                    Kirigami.Separator {
                        Layout.fillWidth: true
                    }

                    // Atmospheric pressure
                    RowLayout {
                        Kirigami.Icon {
                            source: "compass"
                            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                            Layout.minimumWidth: Layout.minimumHeight
                        }
                        Column {
                            Label {
                                font.weight: Font.Bold
                                text: "Pressure"
                            }
                            Label {
                                text: weatherLocation.dayListModel.get(dailyListView.currentIndex).pressure.toFixed(1) + "hPa"
                            }
                        }
                    }

                    Kirigami.Separator {
                        Layout.fillWidth: true
                    }

                    // UV Index
                    RowLayout {
                        Kirigami.Icon {
                            source: "compass"
                            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                            Layout.minimumWidth: Layout.minimumHeight
                        }
                        Column {
                            Label {
                                font.weight: Font.Bold
                                text: "UV index"
                            }
                            Label {
                                text: weatherLocation.dayListModel.get(dailyListView.currentIndex).uvIndex.toFixed(1)
                            }
                        }
                    }
                }
            }
        }
    }
}

