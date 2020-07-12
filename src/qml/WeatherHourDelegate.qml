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
    implicitWidth: Kirigami.Units.gridUnit * 5
    implicitHeight: Kirigami.Units.gridUnit * 10
    color: "transparent"

    property WeatherHour weather

    // actual hour display
    ColumnLayout {
        id: hourElement
        anchors.fill: parent
        anchors.leftMargin: Kirigami.Units.largeSpacing
        anchors.topMargin: Kirigami.Units.largeSpacing
        anchors.rightMargin: Kirigami.Units.largeSpacing
        anchors.bottomMargin: Kirigami.Units.largeSpacing
        spacing: Kirigami.Units.smallSpacing

        Kirigami.Icon {
            source: weather.weatherIcon
            Layout.preferredHeight: Kirigami.Units.iconSizes.medium
            Layout.preferredWidth: Kirigami.Units.iconSizes.medium
        }
        Label {
            text: weather.temperature
            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
        }
        Label {
            text: weather.weatherDescription
        }

        // precipitation
        RowLayout {
            visible: weather != null
            Kirigami.Icon {
                source: "raindrop"
                Layout.preferredHeight: Kirigami.Units.iconSizes.small
                Layout.preferredWidth: Kirigami.Units.iconSizes.small
            }
            Label {
                color: Kirigami.Theme.disabledTextColor
                text: i18n("%1mm", weather.precipitation.toFixed(1))
            }
        }

        // wind
        RowLayout {
            Kirigami.Icon {
                source: "arrow-right"
                Layout.preferredHeight: Kirigami.Units.iconSizes.small
                Layout.preferredWidth: Kirigami.Units.iconSizes.small
            }
            Label {
                color: Kirigami.Theme.disabledTextColor
                text: weather.windSpeed
            }
        }

        Label {
            font.weight: Font.Bold
            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1
            text: weather.date.toLocaleString(Qt.locale(), "h ap").replace(".", "").replace(".", "")
        }
    }
}
