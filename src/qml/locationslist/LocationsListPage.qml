/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2022 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {
    title: i18n("Locations")

    property real yTranslate: 0
    property int currentIndex: 0

    actions: [
        Kirigami.Action {
            icon.name: "list-add"
            text: i18n("Add Location")
            onTriggered: applicationWindow().openAddLocation()
        }
    ]

    LocationsListView {
        transform: Translate {
            y: yTranslate
        }
    }
}
