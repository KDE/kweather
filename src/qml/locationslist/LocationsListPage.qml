/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2022 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.15
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.12 as Kirigami


Kirigami.ScrollablePage {
    title: i18n("Locations")
    
    property real yTranslate: 0
    property int currentIndex: 0

    globalToolBarStyle: Kirigami.ApplicationHeaderStyle.ToolBar
    actions: [
        Kirigami.Action {
            icon.name: "list-add"
            text: i18n("Add Location")
            onTriggered: applicationWindow().openAddLocation()
        }
    ]
    
    LocationsListView {
        transform: Translate { y: yTranslate }
    }
}
