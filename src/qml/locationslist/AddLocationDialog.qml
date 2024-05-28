// Copyright 2022 Devin Lin <espidev@gmail.com>
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Kirigami.Dialog {
    id: root

    standardButtons: Kirigami.Dialog.NoButton
    parent: applicationWindow().overlay
    title: i18n("Add Location")
    preferredHeight: Kirigami.Units.gridUnit * 20
    preferredWidth: Kirigami.Units.gridUnit * 20
    padding: 0

    onOpened: addLocationListView.focusRequested()

    AddLocationListView {
        id: addLocationListView
        clip: true
        onCloseRequested: root.close()
    }
}
