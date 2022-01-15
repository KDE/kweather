/*
    SPDX-FileCopyrightText: 2020 2021 HanY <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

import QtQuick 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.4
import org.kde.plasma.plasmoid 2.0
import org.kde.kirigami 2.11 as Kirigami
Item {
    Plasmoid.backgroundHints: "NoBackground";
    Plasmoid.preferredRepresentation: Plasmoid.compactRepresentation
    
    Plasmoid.fullRepresentation: Loader {
        active: true
        source: plasmoid.nativeInterface.needLocation ? "LocationSelector.qml" : "WeatherContainer.qml"
    }
    
    Plasmoid.compactRepresentation: ColumnLayout{
        Kirigami.Icon {
            height: 20
            width: 20
            source: plasmoid.nativeInterface.weatherIcon
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    plasmoid.expanded = !plasmoid.expanded;
                }
            }
        }
        Label {
            text: plasmoid.nativeInterface.temp + "°"
            color: Kirigami.Theme.activeTextColor
        }
    }
}
