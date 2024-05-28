/*
    SPDX-FileCopyrightText: 2020 2021 HanY <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

import QtQuick 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.4
import org.kde.plasma.plasmoid 2.0
import org.kde.kirigami 2.11 as Kirigami

PlasmoidItem {
    id: root

    Plasmoid.backgroundHints: "ShadowBackground"

    fullRepresentation: Loader {
        active: true
        source: root.plasmoid.needLocation ? "LocationSelector.qml" : "WeatherContainer.qml"
    }

    compactRepresentation: ColumnLayout {
        Kirigami.Icon {
            height: 20
            width: 20
            source: root.plasmoid.weatherIcon
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    plasmoid.expanded = !plasmoid.expanded;
                }
            }
        }
        Label {
            text: root.plasmoid.temp + "°"
            color: Kirigami.Theme.activeTextColor
        }
    }
}
