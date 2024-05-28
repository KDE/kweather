/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2021 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami

import org.kde.kweather.backgrounds
import org.kde.kweather.backgrounds.components

import org.kde.kweather

Kirigami.ScrollablePage {
    id: page

    property int currentIndex: 0
    property var weatherLocation: WeatherLocationListModel.locations[page.currentIndex]
    property var selectedDay: dailyListView.currentItem ? dailyListView.currentItem.weather : weatherLocation.dayForecasts[0]

    property int maximumContentWidth: Kirigami.Units.gridUnit * 35

    // HACK: disable this scrollable when needed because it steals events from dialogs
    flickable.interactive: !applicationWindow().isDialogOpen

    // x-drag threshold to change page
    property real pageChangeThreshold: page.width / 4

    // page functions
    property bool canGoLeft: currentIndex > 0
    property bool canGoRight: currentIndex < WeatherLocationListModel.count - 1
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

    Connections {
        target: WeatherLocationListModel

        function onLocationsChanged() {
            if (page.currentIndex >= WeatherLocationListModel.count) {
                page.currentIndex = WeatherLocationListModel.count - 1;
            }
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
            goLeft ? finishMoveLeft() : finishMoveRight();
        }
    }

    // background
    background: backgroundQml.item // disable OpenGL background for now as it causes issues: backgroundGL.item

    // Loader {
    //     id: backgroundGL
    //     active: !KWEATHER_IS_ANDROID
    //     sourceComponent: WeatherBackground {
    //         anchors.fill: parent
    //         rain: weatherLocation.rain
    //         cloud: weatherLocation.cloud
    //         sun: weatherLocation.sun
    //         star: weatherLocation.star
    //         snow: weatherLocation.snow
    //         colorTop: weatherLocation.topColor
    //         colorBottom: weatherLocation.bottomColor
    //         cloudColor: weatherLocation.cloudColor
    //     }
    // }

    Loader {
        id: backgroundQml
        active: true
        sourceComponent: Rectangle {
            anchors.fill: parent
            color: "#24a3de"

            // background colours
            gradient: Gradient {
                GradientStop {
                    color: backgroundLoader.item ? backgroundLoader.item.gradientColorTop : "white"
                    position: 0.0
                    Behavior on color {
                        ColorAnimation {
                            duration: Kirigami.Units.longDuration
                        }
                    }
                }
                GradientStop {
                    color: backgroundLoader.item ? backgroundLoader.item.gradientColorBottom : "white"
                    position: 1.0
                    Behavior on color {
                        ColorAnimation {
                            duration: Kirigami.Units.longDuration
                        }
                    }
                }
            }

            // separator between the top window decoration bar and the components
            Rectangle {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: 1
                color: Qt.rgba(0, 0, 0, 0.2)
            }

            // background components (ex. cloud, sun, etc.)
            Item {
                anchors.fill: parent
                opacity: {
                    // opacity lightens when you scroll down the page
                    let scrollAmount = page.flickable.contentY - (Kirigami.Units.gridUnit * 3);
                    if (scrollAmount < 0) {
                        scrollAmount = 0;
                    }
                    return 1 - 0.5 * (scrollAmount / (Kirigami.Units.gridUnit * 5));
                }

                // weather elements
                Loader {
                    anchors.fill: parent
                    opacity: backgroundLoader.item && backgroundLoader.item.sun ? 1 : 0
                    Behavior on opacity {
                        NumberAnimation {
                            duration: Kirigami.Units.longDuration
                        }
                    }
                    active: opacity !== 0
                    sourceComponent: Sun {}
                }
                Loader {
                    anchors.fill: parent
                    opacity: backgroundLoader.item && backgroundLoader.item.stars ? 1 : 0
                    Behavior on opacity {
                        NumberAnimation {
                            duration: Kirigami.Units.longDuration
                        }
                    }
                    active: opacity !== 0
                    sourceComponent: Stars {}
                }
                Loader {
                    anchors.fill: parent
                    opacity: backgroundLoader.item && backgroundLoader.item.clouds ? 1 : 0
                    Behavior on opacity {
                        NumberAnimation {
                            duration: Kirigami.Units.longDuration
                        }
                    }
                    active: opacity !== 0
                    sourceComponent: Cloudy {
                        cloudColor: backgroundLoader.item.cloudsColor
                    }
                }
                Loader {
                    anchors.fill: parent
                    opacity: backgroundLoader.item && backgroundLoader.item.rain ? 1 : 0
                    Behavior on opacity {
                        NumberAnimation {
                            duration: Kirigami.Units.longDuration
                        }
                    }
                    active: opacity !== 0
                    sourceComponent: Rain {}
                }
                Loader {
                    anchors.fill: parent
                    opacity: backgroundLoader.item && backgroundLoader.item.snow ? 1 : 0
                    Behavior on opacity {
                        NumberAnimation {
                            duration: Kirigami.Units.longDuration
                        }
                    }
                    active: opacity !== 0
                    sourceComponent: Snow {}
                }

                Loader {
                    id: backgroundLoader
                    anchors.fill: parent
                    Component.onCompleted: {
                        if (weatherLocation) {
                            source = weatherLocation.backgroundComponent;
                        }
                    }

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
    }
    Connections {
        target: weatherLocation
        ignoreUnknownSignals: true // weatherLocation may be null
        function onStopLoadingIndicator() {
            showPassiveNotification(i18n("Weather refreshed for %1", weatherLocation.name));
        }
    }

    Item {
        implicitHeight: mainLayout.implicitHeight

        Rectangle {
            id: rootMask
            color: "transparent"
            opacity: 1 - (Math.abs(x) / (page.width / 4))
            height: parent.height
            width: parent.width

            // left/right dragging for switching pages
            DragHandler {
                id: dragHandler
                target: rootMask
                yAxis.enabled: false
                xAxis.enabled: Kirigami.Settings.hasTransientTouchInput
                xAxis.minimum: page.canGoRight ? -page.width : -pageChangeThreshold / 2 // extra feedback
                xAxis.maximum: page.canGoLeft ? page.width : pageChangeThreshold / 2 // extra feedback

                // HACK: when a delegate, or the listview is being interacted with, disable the DragHandler so that it doesn't switch pages
                enabled: dailyCard.pressedCount == 0 && hourlyCard.pressedCount == 0

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
            }

            // reset to position
            NumberAnimation on x {
                id: xAnim
                to: 0
                running: false
                easing.type: Easing.InOutQuad
                duration: Kirigami.Units.longDuration
            }

            // header
            RowLayout {
                id: header
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: Kirigami.Units.smallSpacing

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: Kirigami.Units.largeSpacing
                    Label {
                        Layout.fillWidth: true
                        font.pointSize: Kirigami.Theme.defaultFont.pointSize * 2
                        font.weight: Font.Bold
                        text: weatherLocation.name
                        color: "white"
                        wrapMode: Text.Wrap
                    }
                    Label {
                        font.pointSize: Kirigami.Theme.defaultFont.pointSize * 0.9
                        color: "white"
                        opacity: 0.9
                        Layout.alignment: Qt.AlignLeft
                        horizontalAlignment: Text.AlignLeft
                        text: i18n("Updated at %1", weatherLocation.lastUpdated)
                    }
                }

                property real buttonLength: Kirigami.Units.gridUnit * 2.5
                property real iconLength: Kirigami.Units.gridUnit * 1.2

                ToolButton {
                    Layout.alignment: Qt.AlignTop
                    Layout.minimumWidth: header.buttonLength
                    Layout.minimumHeight: header.buttonLength

                    Kirigami.Theme.colorSet: Kirigami.Theme.Complementary
                    Kirigami.Theme.inherit: false

                    icon.name: "find-location"
                    icon.height: header.iconLength
                    icon.width: header.iconLength
                    icon.color: "white"

                    text: i18n("Locations")
                    display: ToolButton.IconOnly
                    onClicked: applicationWindow().openLocationsList()

                    ToolTip.visible: down
                    ToolTip.text: i18n("Choose location")
                }
                ToolButton {
                    Layout.alignment: Qt.AlignTop
                    Layout.minimumWidth: header.buttonLength
                    Layout.minimumHeight: header.buttonLength

                    Kirigami.Theme.colorSet: Kirigami.Theme.Complementary
                    Kirigami.Theme.inherit: false

                    icon.name: "settings-configure"
                    icon.height: header.iconLength
                    icon.width: header.iconLength
                    icon.color: "white"

                    text: i18n("Settings")
                    display: ToolButton.IconOnly
                    onClicked: applicationWindow().openSettings()

                    ToolTip.visible: down
                    ToolTip.text: i18n("Settings")
                }
                ToolButton {
                    Layout.alignment: Qt.AlignTop
                    Layout.minimumWidth: header.buttonLength
                    Layout.minimumHeight: header.buttonLength

                    Kirigami.Theme.colorSet: Kirigami.Theme.Complementary
                    Kirigami.Theme.inherit: false

                    icon.name: "view-refresh"
                    icon.height: header.iconLength
                    icon.width: header.iconLength
                    icon.color: "white"

                    visible: !Kirigami.Settings.isMobile
                    text: i18n("Refresh")
                    display: ToolButton.IconOnly
                    onClicked: WeatherLocationListModel.locations[page.currentIndex].update()

                    ToolTip.visible: down
                    ToolTip.text: i18n("Refresh")
                }
            }

            // content
            ColumnLayout {
                id: mainLayout
                anchors.horizontalCenter: parent.horizontalCenter
                width: Math.min(page.width - Kirigami.Units.largeSpacing * 4, maximumContentWidth)

                // separator from top
                // used instead of topMargin, since it can be shrunk when needed (small window height)
                Item {
                    Layout.preferredHeight: Math.max(header.height + Kirigami.Units.gridUnit * 2 // header height
                    , page.height - headerText.height - dailyHeader.height - dailyCard.height - Kirigami.Units.gridUnit * 3) // pin to bottom of window
                }

                // weather header
                ColumnLayout {
                    id: headerText
                    Layout.fillWidth: true
                    RowLayout {
                        spacing: 0
                        Label {
                            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 4
                            font.weight: Font.Light
                            color: "white"
                            text: Math.round(Formatter.convertTemp(weatherLocation.currentHourForecast.temperature, settingsModel.temperatureUnits))
                            font.family: lightHeadingFont.name
                        }
                        Label {
                            Layout.alignment: Qt.AlignTop
                            Layout.topMargin: Kirigami.Units.largeSpacing
                            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 2
                            font.weight: Font.Light
                            color: "white"
                            font.family: lightHeadingFont.name
                            text: Formatter.formatTemperatureUnitDegrees(settingsModel.temperatureUnits)
                        }
                    }
                    Label {
                        font.pointSize: Kirigami.Theme.defaultFont.pointSize * 2
                        font.weight: Font.DemiBold
                        color: "white"
                        Layout.alignment: Qt.AlignLeft
                        horizontalAlignment: Text.AlignLeft
                        text: weatherLocation.currentHourForecast.weatherDescription
                        font.family: lightHeadingFont.name
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
                        text: i18n("Local date: %1", weatherLocation.currentDate)
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

                    property int pressedCount: 0

                    background: Rectangle {
                        color: weatherLocation.cardBackgroundColor
                        radius: Kirigami.Units.smallSpacing
                        anchors.fill: parent
                    }

                    contentItem: WeatherStrip {
                        id: dailyListView
                        selectable: true

                        highlightMoveDuration: 250
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

                        onDraggingChanged: dailyCard.pressedCount += dragging ? 1 : -1

                        model: weatherLocation.dayForecasts
                        delegate: WeatherDayDelegate {
                            id: delegate
                            weather: modelData
                            textColor: weatherLocation.cardTextColor
                            secondaryTextColor: weatherLocation.cardSecondaryTextColor

                            Connections {
                                target: delegate.mouseArea
                                function onPressedChanged() {
                                    dailyCard.pressedCount += delegate.mouseArea.pressed ? 1 : -1;
                                }
                            }
                        }

                        onCurrentIndexChanged: {
                            weatherLocation.selectedDay = currentIndex;
                        }
                    }
                }

                // temperature chart
                TemperatureChartCard {
                    Layout.fillWidth: true
                    location: weatherLocation

                    background: Rectangle {
                        color: weatherLocation.cardBackgroundColor
                        radius: Kirigami.Units.smallSpacing
                        anchors.fill: parent
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
                        text: i18n("Local time: %1", weatherLocation.currentTime)
                        font: Kirigami.Theme.smallFont
                        color: "white"
                        opacity: 0.7
                    }
                }

                // hourly view
                Kirigami.AbstractCard {
                    id: hourlyCard
                    topPadding: Kirigami.Units.gridUnit
                    bottomPadding: Kirigami.Units.gridUnit
                    leftPadding: Kirigami.Units.gridUnit
                    rightPadding: Kirigami.Units.gridUnit
                    Layout.fillWidth: true

                    property int pressedCount: 0

                    background: Rectangle {
                        color: weatherLocation.cardBackgroundColor
                        radius: Kirigami.Units.smallSpacing
                        anchors.fill: parent
                    }

                    contentItem: WeatherStrip {
                        id: hourlyListView
                        selectable: false
                        model: weatherLocation.hourForecasts
                        onDraggingChanged: hourlyCard.pressedCount += dragging ? 1 : -1

                        delegate: WeatherHourDelegate {
                            id: delegate
                            weather: modelData
                            textColor: weatherLocation.cardTextColor
                            secondaryTextColor: weatherLocation.cardSecondaryTextColor

                            Connections {
                                target: delegate.mouseArea
                                function onPressedChanged() {
                                    hourlyCard.pressedCount += delegate.mouseArea.pressed ? 1 : -1;
                                }
                            }
                        }
                    }
                }

                // bottom card (extra info for selected day)
                InfoCard {
                    Layout.fillWidth: true

                    textColor: weatherLocation.cardTextColor

                    background: Rectangle {
                        color: weatherLocation.cardBackgroundColor
                        radius: Kirigami.Units.smallSpacing
                        anchors.fill: parent
                    }
                }

                SunriseCard {
                    Layout.fillWidth: true

                    textColor: weatherLocation.cardTextColor
                    selectedDay: page.selectedDay

                    background: Rectangle {
                        color: weatherLocation.cardBackgroundColor
                        radius: Kirigami.Units.smallSpacing
                        anchors.fill: parent
                    }
                }
            }
        }
    }
}
