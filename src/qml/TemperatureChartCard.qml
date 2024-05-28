/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtCharts
import org.kde.kirigami as Kirigami
import org.kde.kweather
import org.kde.kweather.backgrounds

Kirigami.AbstractCard {
    id: tempChartCard

    required property WeatherLocation location
    property var backgroundColor: location.cardBackgroundColor
    property var textColor: location.cardTextColor

    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0
    implicitHeight: Math.round(Kirigami.Units.gridUnit * 8.5)

    clip: true

    contentItem: ScrollView {
        id: scrollView
        contentHeight: -1
        contentWidth: page.maximumContentWidth

        Item {
            width: page.maximumContentWidth
            height: Math.round(Kirigami.Units.gridUnit * 8.5)

            TemperatureChartData {
                id: chartData
                weatherData: tempChartCard.location.dayForecasts
            }

            ChartView {
                id: chartView
                anchors.fill: parent
                margins.left: Kirigami.Units.largeSpacing
                margins.right: Kirigami.Units.largeSpacing
                margins.top: 0
                margins.bottom: Kirigami.Units.smallSpacing
                legend.visible: false
                antialiasing: true
                localizeNumbers: true
                animationOptions: ChartView.NoAnimation
                backgroundColor: tempChartCard.backgroundColor
                plotAreaColor: tempChartCard.backgroundColor

                height: tempChartCard.height
                width: Math.max(Kirigami.Units.gridUnit * 25, tempChartCard.width)

                SplineSeries {
                    id: splineSeries
                    axisX: DateTimeAxis {
                        id: axisX
                        tickCount: dailyListView.count
                        format: "ddd"
                        labelsColor: tempChartCard.textColor
                        lineVisible: false
                        gridLineColor: Qt.rgba(tempChartCard.textColor.r, tempChartCard.textColor.g, tempChartCard.textColor.b, 0.05)
                    }
                    axisY: ValueAxis {
                        id: axisY
                        visible: false

                        min: chartData.minTempLimit
                        max: chartData.maxTempLimit
                        labelsColor: tempChartCard.textColor
                    }
                    name: i18n("temperature")
                    pointLabelsVisible: true
                    pointLabelsFormat: "@yPoint°"
                    pointLabelsClipping: false
                    pointLabelsColor: tempChartCard.textColor
                    pointLabelsFont.pointSize: Kirigami.Theme.defaultFont.pointSize
                }

                Component.onCompleted: {
                    chartData.initAxes(axisX, axisY);
                    chartData.initSeries(chartView.series(0));
                }
                Component.onDestruction: {
                    // ensure that the series is a nullptr
                    chartData.initSeries(null);
                }
            }

            // allow continuous mouse scrolling
            MouseArea {
                anchors.fill: parent
                property int scrollDist: Kirigami.Units.gridUnit * 2
                onWheel: wheel => {
                    //check if mouse is scrolling up or down
                    if (wheel.angleDelta.y < 0) {
                        page.flickable.contentY += scrollDist;
                    } else {
                        page.flickable.contentY -= scrollDist;
                    }
                }
                onPressed: mouse => mouse.accepted = false // forward mouse event
                onReleased: mouse => mouse.accepted = false // forward mouse event
            }
        }
    }
}
