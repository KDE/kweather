/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.12 as Kirigami

// replace with Kirigami.PlaceholderMessage when it is released
ColumnLayout {
    property alias iconName: placeholderIcon.source
    property alias text: placeholderText.text
    anchors.centerIn: parent
    anchors.margins: Kirigami.Units.largeSpacing
    Kirigami.Icon {
        id: placeholderIcon
        Layout.alignment: Qt.AlignHCenter
        Layout.preferredWidth: Kirigami.Units.iconSizes.huge
        Layout.preferredHeight: Kirigami.Units.iconSizes.huge
        opacity: 0.5
    }
    Kirigami.Heading {
        id: placeholderText
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignHCenter
        horizontalAlignment: Qt.AlignHCenter
        level: 2
        opacity: 0.5
    }
}
