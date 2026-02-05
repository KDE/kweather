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
    readonly property var weatherLocation: WeatherLocationListModel.locations[page.currentIndex]

    readonly property int maximumContentWidth: Kirigami.Units.gridUnit * 35

    // HACK: disable this scrollable when needed because it steals events from dialogs
    flickable.interactive: !applicationWindow().isDialogOpen

    // x-drag threshold to change page
    readonly property real pageChangeThreshold: page.width / 4

    // page functions
    property bool canGoLeft: currentIndex > 0
    property bool canGoRight: currentIndex < WeatherLocationListModel.count - 1
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

    // background
    background: DynamicForecastBackground {
        weatherLocation: page.weatherLocation
    }

    Connections {
        target: WeatherLocationListModel

        function onLocationsChanged() {
            if (page.currentIndex >= WeatherLocationListModel.count) {
                page.currentIndex = WeatherLocationListModel.count - 1;
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

    // Reset to position animation
    NumberAnimation {
        id: xAnim
        target: rootMask; property: "x";
        to: 0
        running: false
        easing.type: Easing.InOutQuad
        duration: Kirigami.Units.longDuration
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
                enabled: !mainLayout.cardPressed

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

            // Header
            DynamicForecastHeader {
                id: header
                weatherLocation: page.weatherLocation
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: Kirigami.Units.smallSpacing
            }

            // Content
            DynamicForecastContent {
                id: mainLayout

                weatherLocation: page.weatherLocation

                topMargin: Math.max(header.height + Kirigami.Units.gridUnit * 2, // header height
                                    page.height - page.topPadding - page.bottomPadding - mainLayout.headerHeight - mainLayout.cardHeight - Kirigami.Units.gridUnit * 2) // pin to bottom of window

                anchors.horizontalCenter: parent.horizontalCenter
                width: Math.min(page.width - Kirigami.Units.largeSpacing * 4, maximumContentWidth)
            }
        }
    }
}
