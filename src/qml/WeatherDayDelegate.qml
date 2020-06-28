/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
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
    color: "transparent"

    property WeatherDay weather

    MouseArea {
        z: 1
        anchors.fill: parent
        onClicked: {
            dailyListView.currentIndex = index
            weatherLocation.hourListModel.updateHourView(index) // change hour view
        }
    }

    // actual day display
    ColumnLayout {
        id: dayElement
        anchors.fill: parent
        anchors.leftMargin: Kirigami.Units.largeSpacing
        anchors.topMargin: Kirigami.Units.largeSpacing
        anchors.rightMargin: Kirigami.Units.largeSpacing
        anchors.bottomMargin: Kirigami.Units.largeSpacing
        spacing: Kirigami.Units.smallSpacing

        Label {
            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1
            text: weather != null ? weather.date.toLocaleString(Qt.locale(), "ddd d").replace(".", "") : ""
        }

        Kirigami.Icon {
            source: weather != null ? weather.weatherIcon : ""
            Layout.preferredHeight: Kirigami.Units.iconSizes.medium
            Layout.preferredWidth: Kirigami.Units.iconSizes.medium
        }
        Row {
            spacing: Kirigami.Theme.defaultFont.pointSize * 0.6
            Label {
                id: highTemp
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
                text: weather != null ? weather.maxTemp : ""
            }
            Label {
                anchors.baseline: highTemp.baseline
                color: Kirigami.Theme.disabledTextColor
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1
                text: weather != null ? weather.minTemp : ""
            }
        }
        Label {
            text: weather != null ? weather.weatherDescription : ""
        }
    }
}
