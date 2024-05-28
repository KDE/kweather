/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes
import org.kde.kirigami as Kirigami
import org.kde.kweather

Item {
    id: root

    implicitWidth: Kirigami.Units.gridUnit * 6
    implicitHeight: dayElement.implicitHeight + Kirigami.Units.largeSpacing * 2

    // implicitHeight: Kirigami.Units.gridUnit * 8

    property var weather
    property color textColor
    property color secondaryTextColor
    property alias mouseArea: mouse

    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: dailyListView.currentIndex = index
    }

    // actual day display
    ColumnLayout {
        id: dayElement
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: Kirigami.Units.largeSpacing
        anchors.topMargin: Kirigami.Units.largeSpacing
        anchors.rightMargin: Kirigami.Units.largeSpacing
        spacing: Kirigami.Units.smallSpacing

        Label {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.smallSpacing
            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1
            text: root.weather.date.toLocaleString(Qt.locale(), "ddd d").replace(".", "")
            color: root.textColor
            elide: Text.ElideRight
        }
        Kirigami.Icon {
            Layout.topMargin: Kirigami.Units.smallSpacing
            source: root.weather.weatherIcon
            implicitHeight: Kirigami.Units.iconSizes.medium
            implicitWidth: Kirigami.Units.iconSizes.medium
        }
        Row {
            Layout.topMargin: Kirigami.Units.smallSpacing
            spacing: Kirigami.Theme.defaultFont.pointSize * 0.6
            Label {
                id: highTemp
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
                text: Formatter.formatTemperatureRounded(root.weather.maxTemp, settingsModel.temperatureUnits)
                color: root.textColor
            }
            Label {
                anchors.baseline: highTemp.baseline
                color: root.secondaryTextColor
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1
                text: Formatter.formatTemperatureRounded(root.weather.minTemp, settingsModel.temperatureUnits)
            }
        }
        Label {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.smallSpacing
            text: root.weather.weatherDescription
            color: root.textColor
            wrapMode: Text.Wrap
        }
    }
}
