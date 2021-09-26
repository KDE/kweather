/*
 * SPDX-FileCopyrightText: 2020-2021 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.11 as Kirigami

Kirigami.GlobalDrawer {
    modal: false
    collapsed: true
    
    Kirigami.Theme.colorSet: Kirigami.Theme.Header
    Kirigami.Theme.inherit: false
    
    actions: [
        Kirigami.Action {
            text: i18n("Forecast")
            iconName: "weather-clear"
            onTriggered: switchToPage(getPage("Forecast"), 0);
        },
        Kirigami.Action {
            text: i18n("Locations")
            iconName: "globe"
            onTriggered: addPageLayer(getPage("Locations"), 0);
        },
        Kirigami.Action {
            text: i18n("Settings")
            iconName: "settings-configure"
            onTriggered: addPageLayer(getPage("Settings"), 0);
        }
    ]
} 
