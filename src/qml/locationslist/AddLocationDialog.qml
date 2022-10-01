// Copyright 2022 Devin Lin <espidev@gmail.com>
// SPDX-License-Identifier: GPL-2.0-or-later

import QtQuick 2.15
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.19 as Kirigami

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
        onCloseRequested: root.close();
    }
}

