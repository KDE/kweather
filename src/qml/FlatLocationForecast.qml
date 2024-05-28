/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes

import org.kde.kirigami as Kirigami
import org.kde.kweather

Kirigami.ScrollablePage {
    id: root

    background: null // transparent, since there is a page behind

    property WeatherLocation weatherLocation
    property var selectedDay: dailyListView.currentItem ? dailyListView.currentItem.weather : weatherLocation.dayForecasts[0]

    property bool inView: false

    // swipe down to refresh
    supportsRefreshing: true
    onRefreshingChanged: {
        if (refreshing) {
            weatherLocation.update();
        }
    }

    Connections {
        target: root.weatherLocation
        ignoreUnknownSignals: true
        function onStopLoadingIndicator() {
            root.refreshing = false;

            // flat mode loads all locations at once, only show one notification for the current item
            if (root.ListView.isCurrentItem) {
                showPassiveNotification(i18n("Weather refreshed for %1", root.weatherLocation.name));
            }
        }
    }

    // all elements are in a column
    ColumnLayout {
        spacing: 0
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Kirigami.Icon {
                id: weatherIcon
                source: root.weatherLocation.currentHourForecast ? root.weatherLocation.currentHourForecast.weatherIcon : "weather-none-available"
                Layout.preferredHeight: width
                Layout.preferredWidth: root.width * 0.8 - headerText.width
                Layout.maximumHeight: Kirigami.Theme.defaultFont.pointSize * 15
                Layout.maximumWidth: Kirigami.Theme.defaultFont.pointSize * 15
                Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 5
                Layout.minimumWidth: Kirigami.Theme.defaultFont.pointSize * 5
                smooth: true
            }

            // weather header
            ColumnLayout {
                id: headerText
                RowLayout {
                    spacing: 0
                    Label {
                        font.pointSize: Kirigami.Theme.defaultFont.pointSize * 3
                        font.weight: Font.Light
                        font.family: lightHeadingFont.name
                        text: root.weatherLocation.currentHourForecast ? Math.round(Formatter.convertTemp(root.weatherLocation.currentHourForecast.temperature, settingsModel.temperatureUnits)) : ""
                    }
                    Label {
                        Layout.alignment: Qt.AlignTop
                        Layout.topMargin: Kirigami.Units.largeSpacing
                        font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
                        font.weight: Font.Light
                        font.family: lightHeadingFont.name
                        text: Formatter.formatTemperatureUnitDegrees(settingsModel.temperatureUnits)
                    }
                }
                Label {
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
                    font.weight: Font.Bold
                    text: root.weatherLocation.currentHourForecast ? root.weatherLocation.currentHourForecast.weatherDescription : ""
                }
                Label {
                    color: Kirigami.Theme.disabledTextColor
                    Layout.topMargin: Kirigami.Units.largeSpacing
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 0.9
                    text: i18n("Updated at %1", root.weatherLocation.lastUpdated)
                }
            }
        }

        PageIndicator {
            visible: Kirigami.Settings.isMobile
            Layout.topMargin: Kirigami.Units.largeSpacing
            Layout.alignment: Qt.AlignHCenter
            opacity: forecastView.count > 1 ? 1 : 0
            count: forecastView.count
            currentIndex: forecastView.currentIndex
        }

        // daily view
        Label {
            text: i18n("Daily")
            font.pointSize: Math.round(Kirigami.Theme.defaultFont.pointSize * 1.2)
            Layout.topMargin: Kirigami.Units.largeSpacing
        }

        Kirigami.Separator {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing * 2
        }

        WeatherStrip {
            id: dailyListView
            selectable: true
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing * 2
            spacing: Kirigami.Units.largeSpacing

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

            model: root.weatherLocation.dayForecasts
            delegate: WeatherDayDelegate {
                weather: modelData
                textColor: Kirigami.Theme.textColor
                secondaryTextColor: Kirigami.Theme.disabledTextColor
            }

            onCurrentIndexChanged: {
                root.weatherLocation.selectedDay = currentIndex;
            }
        }

        // hourly view
        Label {
            Layout.topMargin: Kirigami.Units.largeSpacing * 2
            text: i18n("Hourly")
            font.pointSize: Math.round(Kirigami.Theme.defaultFont.pointSize * 1.2)
        }

        Kirigami.Separator {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing * 2
        }

        WeatherStrip {
            id: hourlyListView
            selectable: false
            implicitHeight: Kirigami.Units.gridUnit * 10.5
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing * 2

            model: root.weatherLocation.hourForecasts

            delegate: WeatherHourDelegate {
                weather: modelData
                textColor: Kirigami.Theme.textColor
                secondaryTextColor: Kirigami.Theme.disabledTextColor
            }
        }

        InfoCard {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing * 2
        }

        SunriseCard {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing * 2
            selectedDay: root.selectedDay
        }
    }
}
