/*
    SPDX-FileCopyrightText: 2020 2021 HanY <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import org.kde.plasma.plasmoid
import org.kde.kirigami as Kirigami

PlasmoidItem {
    Plasmoid.backgroundHints: "ShadowBackground";

    fullRepresentation: Loader {
        active: true
        source: Plasmoid.needLocation ? "LocationSelector.qml" : "WeatherContainer.qml"
    }

    compactRepresentation: ColumnLayout{
        Kirigami.Icon {
            height: 20
            width: 20
            source: Plasmoid.weatherIcon
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    plasmoid.expanded = !plasmoid.expanded;
                }
            }
        }
        Label {
            text: Plasmoid.temp + "°"
            color: Kirigami.Theme.activeTextColor
        }
    }
}
