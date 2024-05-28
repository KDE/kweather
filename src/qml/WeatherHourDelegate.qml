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

Rectangle {
    id: root

    implicitWidth: Kirigami.Units.gridUnit * 5
    implicitHeight: hourElement.height
    color: "transparent"

    property var weather
    property color textColor
    property color secondaryTextColor
    property alias mouseArea: mouse

    MouseArea {
        id: mouse
        anchors.fill: parent
    }

    // actual hour display
    ColumnLayout {
        id: hourElement
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: Kirigami.Units.largeSpacing
        anchors.rightMargin: Kirigami.Units.largeSpacing
        spacing: Kirigami.Units.smallSpacing

        Kirigami.Icon {
            source: root.weather.weatherIcon
            Layout.preferredHeight: Kirigami.Units.iconSizes.medium
            Layout.preferredWidth: Kirigami.Units.iconSizes.medium
        }
        Label {
            Layout.fillWidth: true
            text: Formatter.formatTemperature(root.weather.temperature, settingsModel.temperatureUnits)
            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
            color: root.textColor
            elide: Text.ElideRight
        }
        Item {
            Layout.fillWidth: true
            implicitHeight: descriptionTextMetrics.implicitHeight

            Label {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                text: root.weather.weatherDescription
                color: root.textColor
                wrapMode: Text.Wrap
                elide: Text.ElideRight
                maximumLineCount: 2
            }

            // ensure that the height of the reserved space for the labels is always two text lines
            Label {
                id: descriptionTextMetrics
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                visible: false
                text: 'AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA'
                color: root.textColor
                wrapMode: Text.Wrap
                maximumLineCount: 2
            }
        }

        // precipitation
        RowLayout {
            Kirigami.Icon {
                source: "raindrop"
                Layout.preferredHeight: Kirigami.Units.iconSizes.small
                Layout.preferredWidth: Kirigami.Units.iconSizes.small
                color: settingsModel && settingsModel.forecastStyle === "Dynamic" ? weatherLocation.iconColor : Kirigami.Theme.textColor
                isMask: true
            }
            Label {
                color: root.secondaryTextColor
                text: Formatter.formatPrecipitation(root.weather.precipitationAmount, settingsModel.precipitationUnits)
            }
        }

        // wind
        RowLayout {
            Kirigami.Icon {
                source: "arrow-right"
                Layout.preferredHeight: Kirigami.Units.iconSizes.small
                Layout.preferredWidth: Kirigami.Units.iconSizes.small
                color: settingsModel && settingsModel.forecastStyle === "Dynamic" ? weatherLocation.iconColor : Kirigami.Theme.textColor
                isMask: true
            }
            Label {
                color: root.secondaryTextColor
                text: Formatter.formatWindSpeed(root.weather.windSpeed, settingsModel.speedUnits)
            }
        }

        Label {
            Layout.fillWidth: true
            font.weight: Font.Bold
            text: Formatter.formatHourlyCardDelegateTime(root.weather.date, weatherLocation.timeZone)
            color: root.textColor
            wrapMode: Text.Wrap
        }
    }
}
