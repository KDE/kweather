/*
 * SPDX-FileCopyrightText: 2020-2026 Devin Lin <devin@kde.org>
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

Rectangle {
    id: root

    property WeatherLocation weatherLocation

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

        Loader {
            id: backgroundLoader
            source: weatherLocation.backgroundComponent
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
    }
}
