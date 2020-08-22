/*
    SPDX-FileCopyrightText: 2020 HanY <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

import QtQuick 2.1
import QtQuick.Layouts 1.1
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.kirigami 2.11 as Kirigami

Item {
    Plasmoid.backgroundHints: "NoBackground";
    Plasmoid.fullRepresentation: Item {
        Layout.preferredHeight: Kirigami.Units.gridUnit * 30
        Layout.preferredWidth: Kirigami.Units.gridUnit * 8

        Flow {
            Repeater {
            model: plasmoid.nativeInterface.source()
                Kirigami.Icon {
                    source: modelData
                    width: 50
                    height: 50
                }
            }
        }
    }
}
