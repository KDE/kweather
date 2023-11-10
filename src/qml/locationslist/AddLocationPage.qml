/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2021 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.12 as Kirigami


Kirigami.ScrollablePage {
    id: root
    title: i18n("Add location")
    
    Component.onCompleted: addLocationListView.focusRequested()

    AddLocationListView {
        id: addLocationListView
    }
}

