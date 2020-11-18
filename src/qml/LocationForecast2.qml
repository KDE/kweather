/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
// FIXME: version bump
import org.kde.quickcharts 1.0 as Charts
import org.kde.kirigami 2.13 as Kirigami
import kweather 1.0
import "backgrounds"

Kirigami.ScrollablePage {
    id: page
    property bool inView: false
    verticalScrollBarPolicy: ScrollBar.AlwaysOff
    property WeatherLocation weatherLocation
    property WeatherDay currentDay: weatherLocation.dayListModel.get(dailyListView.currentIndex)

    onInViewChanged: background.item["inView"] = inView

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
            weatherLocation.updateBackend();
        } else {
            showPassiveNotification(i18n("Weather refreshed for %1", weatherLocation.name));
        }
    }
    
    Connections {
        target: weatherLocation
        onStopLoadingIndicator: {page.refreshing = false}
    }

    ColumnLayout {
        id: mainLayout
        Layout.alignment: Qt.AlignHCenter
        width: Math.min(page.width - Kirigami.Units.largeSpacing * 2, Kirigami.Units.gridUnit * 30)
        
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
                text: weatherLocation.currentWeather == null ? "0" : weatherLocation.currentWeather.temperatureRounded
                font.family: lightHeadingFont.name
            }
            Label {
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 2
                font.weight: Font.DemiBold
                color: "white"
                Layout.alignment: Qt.AlignLeft
                horizontalAlignment: Text.AlignLeft
                text: weatherLocation.currentWeather == null ? "Unknown" : weatherLocation.currentWeather.weatherDescription
                font.family: lightHeadingFont.name
            }
            Label {
                Layout.topMargin: Kirigami.Units.largeSpacing
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 0.9
                color: "white"
                Layout.alignment: Qt.AlignLeft
                horizontalAlignment: Text.AlignLeft
                text: i18n("Updated at %1", weatherLocation.lastUpdated)
            }
        }

        // daily view header
        RowLayout {
            id: dailyHeader
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing * 2
            Layout.bottomMargin: Kirigami.Units.largeSpacing
            
            Label {
                text: i18n("Daily")
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
                color: "white"
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
            
            // detect mouse hover
            HoverHandler {
                id: dayMouseArea
                acceptedDevices: PointerDevice.Mouse | PointerDevice.Stylus
            }

            contentItem: ListView {
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
                    weather: dayItem
                    textColor: weatherLocation.cardTextColor
                }
                
                // left right mouse controls
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
        }
        
        // temperature chart
        Rectangle {
            Layout.fillWidth: true
            height: 100
            Charts.LineChart {
                id: tempChart
                anchors.fill: parent
                smooth: true
                colorSource: Charts.SingleValueSource  { value: "red" }
                nameSource: Charts.SingleValueSource  { value: "MaxTemperature" }
//                yRange {
//                    automatic: false
//                    from: 15
//                    to: 30
//                }
                pointDelegate: Label {
                    text: Charts.LineChart.value
                    color: weatherLocation.textColor
                }

                valueSources: [
                    Charts.ArraySource {
                        id: tempSource
                        array: weatherLocation.maxTempList
                    }
                ]
            }
        }
//        ChartView {
//            id: chartView
//            legend.visible: false
//            antialiasing: true
//            animationOptions: ChartView.NoAnimation
//            theme: weatherLocation.darkTheme ? ChartView.ChartThemeDark : ChartView.ChartThemeLight
//            Layout.fillWidth: true
//            height: 200

//            SplineSeries {
//                id: splineSeries
//                axisX: DateTimeAxis {
//                    id: axisX
//                    tickCount: dailyListView.count
//                    format: "ddd"
//                }
//                axisY: ValueAxis {
//                    id: axisY
//                    visible: false

//                    min: weatherLocation.minTempLimit
//                    max: weatherLocation.maxTempLimit
//                }
//                name: i18n("temperature")
//                pointLabelsVisible: true
//                pointLabelsFormat: "@yPoint°"
//                pointLabelsClipping: false

//                Component.onCompleted: {
//                    weatherLocation.initAxes(axisX, axisY);
//                    weatherLocation.initSeries(chartView.series(0));
//                }
//            }
//        }

        // hourly view header
        RowLayout {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing * 2
            Layout.bottomMargin: Kirigami.Units.largeSpacing
            
            Label {
                text: i18n("Hourly")
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
                color: "white"
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

            // detect mouse hover
            HoverHandler {
                id: hourMouseArea
                acceptedDevices: PointerDevice.Mouse | PointerDevice.Stylus
            }
            
            contentItem: ListView {
                id: weatherHourListView
                orientation: ListView.Horizontal

                implicitHeight: Kirigami.Units.gridUnit * 10.5
                implicitWidth: parent.width
                spacing: Kirigami.Units.largeSpacing * 3
                clip: true

                snapMode: ListView.SnapToItem

                model: weatherLocation.hourListModel
                delegate: WeatherHourDelegate {
                    weather: hourItem
                    textColor: weatherLocation.cardTextColor
                }
                
                // left right mouse controls
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
        }
        
        // bottom card (extra info for selected day)
        Kirigami.Card {
            Layout.fillWidth: true

            background: Kirigami.ShadowedRectangle {
                color: weatherLocation.cardBackgroundColor
                radius: Kirigami.Units.smallSpacing
                anchors.fill: parent

                shadow.size: Kirigami.Units.largeSpacing
                shadow.color: Qt.rgba(0.0, 0.0, 0.0, 0.15)
                shadow.yOffset: Kirigami.Units.devicePixelRatio * 2
            }
            contentItem: Item {
                implicitHeight: column.height
                Column {
                    id: column
                    spacing: Kirigami.Units.largeSpacing * 2

                    // precipitation
                    RowLayout {
                        spacing: Kirigami.Units.largeSpacing
                        Kirigami.Icon {
                            source: "raindrop"
                            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                            Layout.minimumWidth: Layout.minimumHeight * 1.5
                            color: weatherLocation.iconColor
                        }
                        Column {
                            spacing: Kirigami.Units.smallSpacing
                            Label {
                                font.weight: Font.Bold
                                text: i18n("Precipitation")
                                color: weatherLocation.cardTextColor
                            }
                            Label {
                                text: currentDay == null ? "" : currentDay.precipitation.toFixed(1) + "mm"
                                color: weatherLocation.cardTextColor
                            }
                        }
                    }

                    // Humidity
                    RowLayout {
                        spacing: Kirigami.Units.largeSpacing
                        Kirigami.Icon {
                            source: "compass"
                            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                            Layout.minimumWidth: Layout.minimumHeight * 1.5
                            color: weatherLocation.iconColor
                        }
                        Column {
                            spacing: Kirigami.Units.smallSpacing
                            Label {
                                font.weight: Font.Bold
                                text: i18n("Humidity")
                                color: weatherLocation.cardTextColor
                            }
                            Label {
                                text: currentDay == null ? "" : i18n("%1%", currentDay.humidity.toFixed(1))
                                color: weatherLocation.cardTextColor
                            }
                        }
                    }

                    // Atmospheric pressure
                    RowLayout {
                        spacing: Kirigami.Units.largeSpacing
                        Kirigami.Icon {
                            source: "speedometer"
                            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                            Layout.minimumWidth: Layout.minimumHeight * 1.5
                            color: weatherLocation.iconColor
                        }
                        Column {
                            spacing: Kirigami.Units.smallSpacing
                            Label {
                                font.weight: Font.Bold
                                text: i18n("Pressure")
                                color: weatherLocation.cardTextColor
                            }
                            Label {
                                text: currentDay == null ? "" : i18n("%1hPa", currentDay.pressure.toFixed(1))
                                color: weatherLocation.cardTextColor
                            }
                        }
                    }

                    // UV Index
                    RowLayout {
                        spacing: Kirigami.Units.largeSpacing
                        Kirigami.Icon {
                            source: "compass"
                            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                            Layout.minimumWidth: Layout.minimumHeight * 1.5
                            color: weatherLocation.iconColor
                        }
                        Column {
                            spacing: Kirigami.Units.smallSpacing
                            Label {
                                font.weight: Font.Bold
                                text: i18n("UV index")
                                color: weatherLocation.cardTextColor
                            }
                            Label {
                                text: currentDay == null ? "" : currentDay.uvIndex.toFixed(1)
                                color: weatherLocation.cardTextColor
                            }
                        }
                    }
                }
            }
        }

        // sunrise/sunset card
        Kirigami.Card {
            Layout.fillWidth: true
            
            background: Kirigami.ShadowedRectangle {
                color: weatherLocation.cardBackgroundColor
                radius: Kirigami.Units.smallSpacing
                anchors.fill: parent

                shadow.size: Kirigami.Units.largeSpacing
                shadow.color: Qt.rgba(0.0, 0.0, 0.0, 0.15)
                shadow.yOffset: Kirigami.Units.devicePixelRatio * 2
            }
            contentItem: Item {
                implicitHeight: sunsetColumn.height

                Column {
                    id: sunsetColumn
                    spacing: Kirigami.Units.largeSpacing * 2

                    // Sunrise
                    RowLayout {
                        spacing: Kirigami.Units.largeSpacing
                        Kirigami.Icon {
                            source: "go-up"
                            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                            Layout.minimumWidth: Layout.minimumHeight * 1.5
                            color: weatherLocation.iconColor
                        }
                        Column {
                            spacing: Kirigami.Units.smallSpacing
                            Label {
                                font.weight: Font.Bold
                                text: i18n("Sunrise")
                                color: weatherLocation.cardTextColor
                            }
                            Label {
                                text: currentDay == null ? "" : currentDay.sunrise
                                color: weatherLocation.cardTextColor
                            }
                        }
                    }

                    // Sunset
                    RowLayout {
                        spacing: Kirigami.Units.largeSpacing
                        Kirigami.Icon {
                            source: "go-down"
                            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                            Layout.minimumWidth: Layout.minimumHeight * 1.5
                            color: weatherLocation.iconColor
                        }
                        Column {
                            spacing: Kirigami.Units.smallSpacing
                            Label {
                                font.weight: Font.Bold
                                text: i18n("Sunset")
                                color: weatherLocation.cardTextColor
                            }
                            Label {
                                text: currentDay == null ? "" : currentDay.sunset
                                color: weatherLocation.cardTextColor
                            }
                        }
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
                                color: weatherLocation.cardTextColor
                            }
                            Label {
                                text: currentDay == null ? "" : currentDay.moonPhase
                                color: weatherLocation.cardTextColor
                            }
                        }
                    }
                }
            }
        }
    }
}
