/*
    SPDX-FileCopyrightText: 2020 HanY <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

import QtQuick 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.4
import org.kde.plasma.plasmoid 2.0
import org.kde.kirigami 2.11 as Kirigami
Item {
    Plasmoid.backgroundHints: "ShadowBackground";
    Plasmoid.fullRepresentation: Loader {
        active: true
        source: plasmoid.nativeInterface.needLocation ? "LocationSelector.qml" : "WeatherContainer.qml"
    }
}
