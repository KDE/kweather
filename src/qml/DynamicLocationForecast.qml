/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtCharts 2.3
import org.kde.kirigami 2.13 as Kirigami
import "backgrounds"
import kweather 1.0

Kirigami.ScrollablePage {
    id: page
    
    property bool inView: false
    property var weatherLocation
    property var currentDay: dailyListView.currentItem.weather
    property var currentHour: hourlyListView.currentItem.weather
    
    property int maximumContentWidth: Kirigami.Units.gridUnit * 35
    
    onInViewChanged: background.item["inView"] = inView
    
    verticalScrollBarPolicy: ScrollBar.AlwaysOff
    
    background: Loader {
        id: background
        anchors.fill: parent
        source: weatherLocation.backgroundComponent
        onSourceChanged: background.item["inView"] = inView
    }
    
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

    Item {
        implicitHeight: mainLayout.implicitHeight
        ColumnLayout {
            id: mainLayout
            anchors.horizontalCenter: parent.horizontalCenter
            width: Math.min(page.width - Kirigami.Units.largeSpacing * 4, maximumContentWidth)
            
            // separator from top
            Item {
                Layout.preferredHeight: page.height - headerText.height - dailyHeader.height - dailyCard.height - Kirigami.Units.gridUnit * 3
            }
            
            // weather header
            ColumnLayout {
                id: headerText
                Layout.fillWidth: true
                Label {
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 4
                    font.weight: Font.Light
                    color: "white"
                    Layout.alignment: Qt.AlignLeft
                    horizontalAlignment: Text.AlignLeft
                    text: Formatter.formatTemperatureRounded(page.currentHour.temperature)
                    font.family: lightHeadingFont.name
                }
                Label {
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 2
                    font.weight: Font.DemiBold
                    color: "white"
                    Layout.alignment: Qt.AlignLeft
                    horizontalAlignment: Text.AlignLeft
                    text: page.currentHour.weatherDescription
                    font.family: lightHeadingFont.name
                }
                Label {
                    Layout.topMargin: Kirigami.Units.largeSpacing
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 0.9
                    color: "white"
                    opacity: 0.7
                    Layout.alignment: Qt.AlignLeft
                    horizontalAlignment: Text.AlignLeft
                    text: i18n("Updated at %1", weatherLocation.lastUpdated)
                }
            }

            // daily view header
            ColumnLayout {
                id: dailyHeader
                spacing: Kirigami.Units.smallSpacing
                Layout.fillWidth: true
                Layout.topMargin: Kirigami.Units.largeSpacing * 2
                Layout.bottomMargin: Kirigami.Units.largeSpacing
                
                Label {
                    text: i18n("Daily")
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
                    color: "white"
                }
                Label {
                    text: i18n("Local Date: ") + weatherLocation.currentDate
                    font: Kirigami.Theme.smallFont
                    color: "white"
                    opacity: 0.7
                }
            }
            
            // daily view
            Control {
                id: dailyCard
                Layout.fillWidth: true
                padding: Kirigami.Units.largeSpacing
                
                background: Kirigami.ShadowedRectangle {
                    color: weatherLocation.cardBackgroundColor
                    radius: Kirigami.Units.smallSpacing
                    anchors.fill: parent

                    shadow.size: Kirigami.Units.largeSpacing
                    shadow.color: Qt.rgba(0.0, 0.0, 0.0, 0.15)
                    shadow.yOffset: Kirigami.Units.devicePixelRatio * 2
                }

                contentItem: WeatherStrip {
                    id: dailyListView

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

                    spacing: Kirigami.Units.largeSpacing

                    model: weatherLocation.dayForecasts
                    delegate: WeatherDayDelegate {
                        weather: modelData
                        textColor: weatherLocation.cardTextColor
                    }

                    onCurrentIndexChanged: {
                        weatherLocation.selectedDay = currentIndex
                    }
                }
            }
            
            // temperature chart
            TemperatureChartCard {
                location: weatherLocation
            }

            // hourly view header
            ColumnLayout {
                spacing: Kirigami.Units.smallSpacing
                Layout.fillWidth: true
                Layout.topMargin: Kirigami.Units.largeSpacing * 2
                Layout.bottomMargin: Kirigami.Units.largeSpacing
                
                Label {
                    text: i18n("Hourly")
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
                    color: "white"
                }
                Label {
                    text: i18n("Local Time: ") + weatherLocation.currentTime
                    font: Kirigami.Theme.smallFont
                    color: "white"
                    opacity: 0.7
                }
            }

            // hourly view
            Kirigami.Card {
                id: hourlyCard
                Layout.fillWidth: true
                
                background: Kirigami.ShadowedRectangle {
                    color: weatherLocation.cardBackgroundColor
                    radius: Kirigami.Units.smallSpacing
                    anchors.fill: parent

                    shadow.size: Kirigami.Units.largeSpacing
                    shadow.color: Qt.rgba(0.0, 0.0, 0.0, 0.15)
                    shadow.yOffset: Kirigami.Units.devicePixelRatio * 2
                }

                contentItem: WeatherStrip {
                    id: hourlyListView
                    model: weatherLocation.hourForecasts

                    delegate: WeatherHourDelegate {
                        weather: modelData
                        textColor: weatherLocation.cardTextColor
                    }
                }
            }
            
            // bottom card (extra info for selected day)
            InfoCard {
                Layout.fillWidth: true

                textColor: weatherLocation.cardTextColor

                background: Kirigami.ShadowedRectangle {
                    color: weatherLocation.cardBackgroundColor
                    radius: Kirigami.Units.smallSpacing
                    anchors.fill: parent

                    shadow.size: Kirigami.Units.largeSpacing
                    shadow.color: Qt.rgba(0.0, 0.0, 0.0, 0.15)
                    shadow.yOffset: Kirigami.Units.devicePixelRatio * 2
                }
            }

            SunriseCard {
                Layout.fillWidth: true

                textColor: weatherLocation.cardTextColor

                background: Kirigami.ShadowedRectangle {
                    color: weatherLocation.cardBackgroundColor
                    radius: Kirigami.Units.smallSpacing
                    anchors.fill: parent

                    shadow.size: Kirigami.Units.largeSpacing
                    shadow.color: Qt.rgba(0.0, 0.0, 0.0, 0.15)
                    shadow.yOffset: Kirigami.Units.devicePixelRatio * 2
                }
            }
        }
    }
}
