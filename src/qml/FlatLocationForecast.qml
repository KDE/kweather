/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtQuick.Shapes 1.12
import org.kde.kirigami 2.11 as Kirigami
import kweather 1.0

Kirigami.ScrollablePage {
    id: page
    verticalScrollBarPolicy: ScrollBar.AlwaysOff

    background: null // transparent, since there is a page behind

    property var weatherLocation
    property var currentDay: dailyListView.currentItem.weather
    property var currentHour: hourlyListView.currentItem.weather

    property bool inView: false

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

    // all elements are in a column
    ColumnLayout {
        spacing: 0
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            Kirigami.Icon {
                id: weatherIcon
                source: page.currentHour.weatherIcon
                Layout.preferredHeight: width
                Layout.preferredWidth: page.width * 0.8 - headerText.width
                Layout.maximumHeight: Kirigami.Theme.defaultFont.pointSize * 15
                Layout.maximumWidth: Kirigami.Theme.defaultFont.pointSize * 15
                Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 5
                Layout.minimumWidth: Kirigami.Theme.defaultFont.pointSize * 5
                smooth: true
            }

            // weather header
            ColumnLayout {
                id: headerText
                Label {
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 3
                    font.weight: Font.Light
                    font.family: lightHeadingFont.name
                    text: Formatter.formatTemperatureRounded(page.currentHour.temperature, settingsModel.temperatureUnits)
                }
                Label {
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
                    font.weight: Font.Bold
                    text: page.currentHour.weatherDescription
                }
                Label {
                    color: Kirigami.Theme.disabledTextColor
                    Layout.topMargin: Kirigami.Units.largeSpacing
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 0.9
                    text: i18n("Updated at %1", weatherLocation.lastUpdated)
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
            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
            Layout.topMargin: Kirigami.Units.largeSpacing
        }
        Kirigami.Separator {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing * 2
        }

        WeatherStrip {
            id: dailyListView
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing * 2
            implicitHeight: Kirigami.Units.gridUnit * 8
            spacing: Kirigami.Units.largeSpacing

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

            model: weatherLocation.dayForecasts
            delegate: WeatherDayDelegate {
                weather: modelData
                textColor: Kirigami.Theme.textColor
            }

            onCurrentIndexChanged: {
                weatherLocation.selectedDay = currentIndex
            }
        }

        // hourly view
        Label {
            Layout.topMargin: Kirigami.Units.largeSpacing * 2
            text: i18n("Hourly")
            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
        }
        Kirigami.Separator {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing * 2
        }

        WeatherStrip {
            id: hourlyListView
            implicitHeight: Kirigami.Units.gridUnit * 10.5
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing * 2

            model: weatherLocation.hourForecasts

            delegate: WeatherHourDelegate {
                weather: modelData
                textColor: Kirigami.Theme.textColor
            }
        }

        InfoCard {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing * 2
        }

        SunriseCard {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing * 2
        }
    }
}

