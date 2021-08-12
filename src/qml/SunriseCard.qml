/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtCharts 2.3
import org.kde.kirigami 2.13 as Kirigami
import "backgrounds"

Kirigami.Card {

    id: root

    property var textColor: Kirigami.Theme.textColor

    contentItem: Item {
        implicitHeight: sunsetColumn.height

        Column {
            id: sunsetColumn
            spacing: Kirigami.Units.largeSpacing * 2

            // Sunrise
            RowLayout {
                spacing: Kirigami.Units.largeSpacing
                Kirigami.Icon {
                    source: "go-up"
                    Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                    Layout.minimumWidth: Layout.minimumHeight * 1.5
                    color: weatherLocation.iconColor
                }
                Column {
                    spacing: Kirigami.Units.smallSpacing
                    Label {
                        font.weight: Font.Bold
                        text: i18n("Sunrise")
                        color: root.textColor
                    }
                    Label {
                        text: Qt.formatTime(currentDay.sunrise.sunRise, "hh:mm ap")
                        color: root.textColor
                    }
                }
            }

            // Sunset
            RowLayout {
                spacing: Kirigami.Units.largeSpacing
                Kirigami.Icon {
                    source: "go-down"
                    Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                    Layout.minimumWidth: Layout.minimumHeight * 1.5
                    color: weatherLocation.iconColor
                }
                Column {
                    spacing: Kirigami.Units.smallSpacing
                    Label {
                        font.weight: Font.Bold
                        text: i18n("Sunset")
                        color: root.textColor
                    }
                    Label {
                        text: Qt.formatTime(currentDay.sunrise.sunSet, "hh:mm ap")
                        color: root.textColor
                    }
                }
            }

            // Moon phase
            RowLayout {
                spacing: Kirigami.Units.largeSpacing
                Kirigami.Icon {
                    source: "weather-clear-night"
                    Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                    Layout.minimumWidth: Layout.minimumHeight * 1.5
                }
                Column {
                    spacing: Kirigami.Units.smallSpacing
                    Label {
                        font.weight: Font.Bold
                        text: i18n("Moon Phase")
                        color: root.textColor
                    }
                    Label {
                        text: currentDay.sunrise.moonPhaseString
                        color: root.textColor
                    }
                }
            }
        }
    }
}
