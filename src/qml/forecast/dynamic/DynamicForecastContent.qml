/*
 * SPDX-FileCopyrightText: 2020-2026 Devin Lin <devin@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami

import org.kde.kweather

ColumnLayout {
    id: root

    property real topMargin
    property WeatherLocation weatherLocation

    property var selectedDay: weatherLocation.dayForecasts[weatherLocation.selectedDay]
    readonly property real headerHeight: headerText.height
    readonly property real cardHeight: dailyCard.height
    readonly property bool cardPressed: dailyCard.pressedCount > 0 || hourlyCard.pressedCount > 0

    onWeatherLocationChanged: {
        // HACK: for some reason the models don't get updated automatically
        dailyListView.model = [];
        dailyListView.model = Qt.binding(() => weatherLocation.dayForecasts);

        selectedDay = null;
        selectedDay = Qt.binding(() => weatherLocation.dayForecasts[weatherLocation.selectedDay]);
    }

    Item {
        Layout.preferredHeight: root.topMargin
    }

    // weather header
    ColumnLayout {
        id: headerText
        Layout.alignment: Qt.AlignHCenter
        Layout.fillWidth: true
        Layout.bottomMargin: Kirigami.Units.gridUnit * 2
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 0
            Label {
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 6
                font.weight: Font.DemiBold
                color: "white"
                text: Math.round(Formatter.convertTemp(weatherLocation.currentHourForecast.temperature, settingsModel.temperatureUnits))
                font.family: lightHeadingFont.name
            }
            Label {
                Layout.alignment: Qt.AlignTop
                Layout.topMargin: Kirigami.Units.largeSpacing * 2
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 3
                font.weight: Font.Light
                color: "white"
                font.family: lightHeadingFont.name
                text: Formatter.formatTemperatureUnitDegrees(settingsModel.temperatureUnits)
            }
        }
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: Kirigami.Units.gridUnit / 2
            Kirigami.Icon {
                id: weatherDescriptionIcon
                implicitHeight: Kirigami.Units.iconSizes.medium
                implicitWidth: Kirigami.Units.iconSizes.medium
                source: weatherLocation.currentHourForecast.weatherIcon + "-symbolic"
                color: "white"
            }
            Label {
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
                font.weight: Font.DemiBold
                color: "white"
                text: weatherLocation.currentHourForecast.weatherDescription
                font.family: lightHeadingFont.name
            }
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
            radius: Kirigami.Units.cornerRadius
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
                radius: Kirigami.Units.cornerRadius
                opacity: 0.3
                focus: true
            }

            spacing: Kirigami.Units.largeSpacing

            onDraggingChanged: dailyCard.pressedCount += dragging ? 1 : -1

            model: root.weatherLocation.dayForecasts
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
            radius: Kirigami.Units.cornerRadius
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
            radius: Kirigami.Units.cornerRadius
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
        selectedDay: root.selectedDay

        background: Rectangle {
            color: weatherLocation.cardBackgroundColor
            radius: Kirigami.Units.smallSpacing
            anchors.fill: parent
        }
    }
}
