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
import kweather 1.0
import "backgrounds"

Kirigami.ScrollablePage {
    id: page
    
    property bool inView: false
    property WeatherLocation weatherLocation
    property var currentDay: dailyListView.currentItem.weather
    
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
                    text: weatherLocation.hourListModel.currentForecast == null ? "0" : weatherLocation.hourListModel.currentForecast.temperatureRounded
                    font.family: lightHeadingFont.name
                }
                Label {
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 2
                    font.weight: Font.DemiBold
                    color: "white"
                    Layout.alignment: Qt.AlignLeft
                    horizontalAlignment: Text.AlignLeft
                    text: weatherLocation.hourListModel.currentForecast == null ? "Unknown" : weatherLocation.hourListModel.currentForecast.weatherDescription
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

                    model: weatherLocation.dayForecasts
                    delegate: WeatherDayDelegate {
                        weather: modelData
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
            Control {
                id: tempChartCard
                
                Layout.fillWidth: true
                leftPadding: 0; rightPadding: 0; topPadding: 0; bottomPadding: 0;
                implicitHeight: Math.round(Kirigami.Units.gridUnit * 8.5)

                background: Kirigami.ShadowedRectangle {
                    color: weatherLocation.cardBackgroundColor
                    radius: Kirigami.Units.smallSpacing
                    anchors.fill: parent

                    shadow.size: Kirigami.Units.largeSpacing
                    shadow.color: Qt.rgba(0.0, 0.0, 0.0, 0.15)
                    shadow.yOffset: Kirigami.Units.devicePixelRatio * 2
                }

                contentItem: ScrollView {
                    id: scrollView
                    contentHeight: -1
                    contentWidth: page.maximumContentWidth

                    Item {
                        width: page.maximumContentWidth
                        height: Math.round(Kirigami.Units.gridUnit * 8.5)
                        
                        ChartView {
                            id: chartView
                            anchors.fill: parent
                            margins.left: Kirigami.Units.largeSpacing; margins.right: Kirigami.Units.largeSpacing
                            margins.top: 0; margins.bottom: Kirigami.Units.smallSpacing
                            legend.visible: false
                            antialiasing: true
                            
                            animationOptions: ChartView.NoAnimation
                            backgroundColor: weatherLocation.cardBackgroundColor
                            plotAreaColor: weatherLocation.cardBackgroundColor
                            
                            height: tempChartCard.height
                            width: Math.max(Kirigami.Units.gridUnit * 25, tempChartCard.width)

                            SplineSeries {
                                id: splineSeries
                                axisX: DateTimeAxis {
                                    id: axisX
                                    tickCount: dailyListView.count
                                    format: "ddd"
                                    labelsColor: weatherLocation.cardTextColor
                                    lineVisible: false
                                    gridLineColor: Qt.rgba(weatherLocation.cardTextColor.r, weatherLocation.cardTextColor.g, weatherLocation.cardTextColor.b, 0.05)
                                }
                                axisY: ValueAxis {
                                    id: axisY
                                    visible: false

                                    min: weatherLocation.minTempLimit
                                    max: weatherLocation.maxTempLimit
                                    labelsColor: weatherLocation.cardTextColor
                                }
                                name: i18n("temperature")
                                pointLabelsVisible: true
                                pointLabelsFormat: "@yPoint°"
                                pointLabelsClipping: false
                                pointLabelsColor: weatherLocation.cardTextColor
                                pointLabelsFont.pointSize: Kirigami.Theme.defaultFont.pointSize
                            }

                            Component.onCompleted: {
                                weatherLocation.initAxes(axisX, axisY);
                                weatherLocation.initSeries(chartView.series(0));
                            }
                        }
                        
                        // allow continuous mouse scrolling
                        MouseArea {
                            anchors.fill: parent
                            property int scrollDist: Kirigami.Units.gridUnit * 2
                            onWheel: {
                                //check if mouse is scrolling up or down
                                if (wheel.angleDelta.y < 0){
                                    page.flickable.contentY += scrollDist
                                } else {
                                    page.flickable.contentY -= scrollDist
                                }
                            }
                            onPressed: mouse.accepted = false // forward mouse event
                            onReleased: mouse.accepted = false // forward mouse event
                        }
                    }
                }
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

                // detect mouse hover
                HoverHandler {
                    id: hourMouseArea
                    acceptedDevices: PointerDevice.Mouse | PointerDevice.Stylus
                }
                
                contentItem: ListView {
                    id: weatherHourListView
                    orientation: ListView.Horizontal

                    implicitHeight: contentItem.childrenRect.height
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
                                    text: Qt.formatTime(currentDay.sunrise.sunRise, Locale.ShortFormat)
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
                                    text: Qt.formatTime(currentDay.sunrise.sunSet, Locale.ShortFormat)
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
                                    text: currentDay.sunrise.moonPhaseString
                                    color: weatherLocation.cardTextColor
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
