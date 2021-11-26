/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtQuick.Shapes 1.12
import org.kde.kirigami 2.11 as Kirigami
import kweather 1.0

Rectangle {
    implicitWidth: Kirigami.Units.gridUnit * 6
    implicitHeight: Kirigami.Units.gridUnit * 8

    property var weather
    property color textColor
    property color secondaryTextColor
    property alias mouseArea: mouse 
    
    color: "transparent"
    
    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: dailyListView.currentIndex = index
    }
    
    // actual day display
    ColumnLayout {
        id: dayElement
        anchors.fill: parent
        anchors.leftMargin: Kirigami.Units.largeSpacing
        anchors.topMargin: Kirigami.Units.largeSpacing
        anchors.rightMargin: Kirigami.Units.largeSpacing
        anchors.bottomMargin: Kirigami.Units.largeSpacing
        spacing: 0

        Label {
            Layout.topMargin: Kirigami.Units.smallSpacing
            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1
            text: weather.date.toLocaleString(Qt.locale(), "ddd d").replace(".", "")
            color: textColor
        }
        Kirigami.Icon {
            Layout.topMargin: Kirigami.Units.smallSpacing
            source: weather.weatherIcon
            implicitHeight: Kirigami.Units.iconSizes.medium
            implicitWidth: Kirigami.Units.iconSizes.medium
        }
        Row {
            Layout.topMargin: Kirigami.Units.smallSpacing
            spacing: Kirigami.Theme.defaultFont.pointSize * 0.6
            Label {
                id: highTemp
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
                text: Formatter.formatTemperatureRounded(weather.maxTemp, settingsModel.temperatureUnits)
                color: textColor
            }
            Label {
                anchors.baseline: highTemp.baseline
                color: secondaryTextColor
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1
                text: Formatter.formatTemperatureRounded(weather.minTemp, settingsModel.temperatureUnits)
            }
        }
        Label {
            Layout.topMargin: Kirigami.Units.smallSpacing
            text: weather.weatherDescription
            color: textColor
        }
    }
}
