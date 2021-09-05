/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtCharts 2.3
import org.kde.kirigami 2.13 as Kirigami
import "backgrounds"
import "backgrounds/components"
import kweather 1.0

Kirigami.ScrollablePage {
    id: page
    
    property int currentIndex: 0
    property var weatherLocation: weatherLocationListModel.locations[loader.item.currentIndex]
    property var selectedDay: dailyListView.currentItem ? dailyListView.currentItem.weather : weatherLocation.todayForecast
    
    property int maximumContentWidth: Kirigami.Units.gridUnit * 35
    
    verticalScrollBarPolicy: ScrollBar.AlwaysOff
    
    // x-drag threshold to change page
    property real pageChangeThreshold: page.width / 4
    
    // page functions
    property bool canGoLeft: currentIndex > 0
    property bool canGoRight: currentIndex < weatherLocationListModel.count - 1
    function moveLeft() {
        if (page.canGoLeft) {
            xOutAnim.goLeft = true;
            xOutAnim.to = pageChangeThreshold;
            xOutAnim.restart();
        }
    }
    function moveRight() {
        if (page.canGoRight) {
            xOutAnim.goLeft = false;
            xOutAnim.to = -pageChangeThreshold;
            xOutAnim.restart();
        }
    }
    function finishMoveLeft() {
        if (page.canGoLeft) {
            --currentIndex;
            // animate as if new cards are coming in from the screen side
            rootMask.x = -pageChangeThreshold;
            xAnim.restart();
        }
    }
    function finishMoveRight() {
        if (page.canGoRight) {
            ++currentIndex;
            // animate as if new cards are coming in from the screen side
            rootMask.x = pageChangeThreshold;
            xAnim.restart();
        }
    }
    
    // animate x fade out before page switch
    NumberAnimation {
        id: xOutAnim
        target: rootMask
        property: "x"
        easing.type: Easing.InOutQuad
        duration: Kirigami.Units.longDuration
        property bool goLeft: false
        onFinished: {
            if (goLeft) finishMoveLeft();
            else finishMoveRight();
        }
    }
    
    // background
    background: Rectangle {
        anchors.fill: parent
        color: "#24a3de"
        
        // background colours
        gradient: Gradient {
            GradientStop { 
                color: backgroundLoader.item.gradientColorTop
                position: 0.0
                Behavior on color {
                    ColorAnimation { duration: Kirigami.Units.longDuration }
                }
            }
            GradientStop { 
                color: backgroundLoader.item.gradientColorBottom
                position: 1.0 
                Behavior on color {
                    ColorAnimation { duration: Kirigami.Units.longDuration }
                }
            }
        }
        
        // background components (ex. cloud, sun, etc.)
        Item {
            anchors.fill: parent
            opacity: { // opacity lightens when you scroll down the page
                let scrollAmount = page.flickable.contentY - (Kirigami.Units.gridUnit * 3);
                if (scrollAmount < 0) {
                    scrollAmount = 0;
                }
                
                return 1 - 0.5 * (scrollAmount / (Kirigami.Units.gridUnit * 5));
            }
            
            // weather elements
            Loader {
                anchors.fill: parent
                active: backgroundLoader.item.sun
                Component.onCompleted: console.log("sun: " + backgroundLoader.item.sun)
                sourceComponent: Sun {}
            }
            Loader {
                anchors.fill: parent
                Component.onCompleted: console.log("stars: " + backgroundLoader.item.stars)
                active: backgroundLoader.item.stars && !weatherLocationListModel.isLowPower
                sourceComponent: Stars {}
            }
            Loader {
                anchors.fill: parent
                active: backgroundLoader.item.clouds
                Component.onCompleted: console.log("clouds: " + backgroundLoader.item.clouds)
                sourceComponent: Cloudy { cloudColor: backgroundLoader.item.cloudsColor }
            }
            Loader {
                anchors.fill: parent
                Component.onCompleted: console.log("rain: " + backgroundLoader.item.rain)
                active: backgroundLoader.item.rain
                sourceComponent: Rain {}
            }
            Loader {
                anchors.fill: parent
                Component.onCompleted: console.log("snow: " + backgroundLoader.item.snow)
                active: backgroundLoader.item.snow
                sourceComponent: Snow {}
            }
            
            Loader {
                id: backgroundLoader
                anchors.fill: parent
                Component.onCompleted: source = weatherLocation.backgroundComponent
                
                NumberAnimation {
                    id: backgroundLoaderOpacity
                    target: backgroundLoader.item
                    property: "opacity"
                    duration: Kirigami.Units.longDuration
                    to: 1
                    running: true
                    onFinished: {
                        backgroundLoader.source = weatherLocation.backgroundComponent;
                        to = 1;
                        restart();
                    }
                }
            }
        }
        
        // fade away background when locations changed
        Connections {
            target: page
            function onCurrentIndexChanged() {
                backgroundLoaderOpacity.to = 0;
                backgroundLoaderOpacity.restart();
            }
        }
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
    
    Rectangle {
        id: rootMask
        color: "transparent"
        opacity: 1 - (Math.abs(x) / (page.width / 4))
        implicitHeight: mainLayout.implicitHeight
        
        // left/right dragging
        DragHandler {
            id: dragHandler
            target: rootMask
            yAxis.enabled: false; xAxis.enabled: true
            xAxis.minimum: page.canGoRight ? -page.width : -pageChangeThreshold / 2 // extra feedback
            xAxis.maximum: page.canGoLeft ? page.width : pageChangeThreshold / 2 // extra feedback
            onActiveChanged: {
                if (!active) {
                    // if drag passed threshold, change page
                    if (rootMask.x >= pageChangeThreshold) {
                        page.finishMoveLeft();
                    } else if (rootMask.x <= -pageChangeThreshold) {
                        page.finishMoveRight();
                    } else {
                        xAnim.restart(); // reset position
                    }
                }
            }
            
            //grabPermissions: PointerHandler.ApprovesTakeOverByAnything | PointerHandler.CanTakeOverFromHandlersOfDifferentType | PointerHandler.ApprovesTakeOverByHandlersOfDifferentType
        }
        // reset to position
        NumberAnimation on x {
            id: xAnim; to: 0
            easing.type: Easing.InOutQuad
            duration: Kirigami.Units.longDuration
        }
        
        ColumnLayout {
            id: mainLayout
            anchors.horizontalCenter: parent.horizontalCenter
            width: Math.min(page.width - Kirigami.Units.largeSpacing * 4, maximumContentWidth)
            
            // weather header
            ColumnLayout {
                id: headerText
                Layout.topMargin: page.height - headerText.height - dailyHeader.height - dailyCard.height - Kirigami.Units.gridUnit * 3
                Layout.fillWidth: true
                Label {
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 4
                    font.weight: Font.Light
                    color: "white"
                    Layout.alignment: Qt.AlignLeft
                    horizontalAlignment: Text.AlignLeft
                    text: Formatter.formatTemperatureRounded(weatherLocation.currentHour.temperature, settingsModel.temperatureUnits)
                    font.family: lightHeadingFont.name
                }
                Label {
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 2
                    font.weight: Font.DemiBold
                    color: "white"
                    Layout.alignment: Qt.AlignLeft
                    horizontalAlignment: Text.AlignLeft
                    text: weatherLocation.currentHour.weatherDescription
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
                    selectable: true

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
                    selectable: false
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

