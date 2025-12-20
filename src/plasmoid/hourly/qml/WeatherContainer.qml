/*
    SPDX-FileCopyrightText: 2021 HanY <hanyoung@protonmail.com>
    SPDX-FileCopyrightText: 2025 Devin Lin <devin@kde.org>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import org.kde.plasma.plasmoid
import org.kde.kirigami as Kirigami
import org.kde.kirigami.delegates as KD

Rectangle {
    id: container
    clip: true

    Layout.preferredWidth: Kirigami.Units.gridUnit * 12
    Layout.preferredHeight: Kirigami.Units.gridUnit * 12

    radius: Kirigami.Units.gridUnit
    color: Kirigami.Theme.backgroundColor

    gradient: Plasmoid.topColor === Qt.rgba(0, 0, 0, 0) ? null : candidateGradient

    property Gradient candidateGradient: Gradient {
        GradientStop {
            color: Plasmoid.topColor
            position: 0.0
        }
        GradientStop {
            color: Plasmoid.bottomColor
            position: 1.0
        }
    }

    Component {
        id: weatherWidget
        WeatherWidget {}
    }

    Component {
        id: placeholder
        Item {
            Kirigami.PlaceholderMessage {
                anchors.centerIn: parent
                text: i18n("Set a location")
                icon.name: "globe"
                helpfulAction: Kirigami.Action {
                    text: i18n("Open settings")
                    onTriggered: {
                        Plasmoid.internalAction("configure").trigger();
                    }
                }
            }
        }
    }

    Loader {
        anchors.fill: parent
        sourceComponent: Plasmoid.needLocation ? placeholder : weatherWidget
    }
}
