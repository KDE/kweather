/*
 * SPDX-FileCopyrightText: 2020-2026 Devin Lin <devin@kde.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami

import org.kde.kweather

RowLayout {
    id: header

    property WeatherLocation weatherLocation

    ColumnLayout {
        Layout.fillWidth: true
        spacing: Kirigami.Units.largeSpacing
        Label {
            Layout.fillWidth: true
            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 2
            font.weight: Font.Bold
            text: weatherLocation.name
            color: "white"
            wrapMode: Text.Wrap
        }
        Label {
            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 0.9
            color: "white"
            opacity: 0.9
            Layout.alignment: Qt.AlignLeft
            horizontalAlignment: Text.AlignLeft
            text: i18n("Updated at %1", weatherLocation.lastUpdated)
        }
    }

    property real buttonLength: Kirigami.Units.gridUnit * 2.5
    property real iconLength: Kirigami.Units.gridUnit * 1.2

    ToolButton {
        Layout.alignment: Qt.AlignTop
        Layout.minimumWidth: header.buttonLength
        Layout.minimumHeight: header.buttonLength

        Kirigami.Theme.colorSet: Kirigami.Theme.Complementary
        Kirigami.Theme.inherit: false

        icon.name: "find-location"
        icon.height: header.iconLength
        icon.width: header.iconLength
        icon.color: "white"

        text: i18n("Locations")
        display: ToolButton.IconOnly
        onClicked: applicationWindow().openLocationsList()

        ToolTip.visible: down
        ToolTip.text: i18n("Choose location")
    }
    ToolButton {
        Layout.alignment: Qt.AlignTop
        Layout.minimumWidth: header.buttonLength
        Layout.minimumHeight: header.buttonLength

        Kirigami.Theme.colorSet: Kirigami.Theme.Complementary
        Kirigami.Theme.inherit: false

        icon.name: "settings-configure"
        icon.height: header.iconLength
        icon.width: header.iconLength
        icon.color: "white"

        text: i18n("Settings")
        display: ToolButton.IconOnly
        onClicked: applicationWindow().openSettings()

        ToolTip.visible: down
        ToolTip.text: i18n("Settings")
    }
    ToolButton {
        Layout.alignment: Qt.AlignTop
        Layout.minimumWidth: header.buttonLength
        Layout.minimumHeight: header.buttonLength

        Kirigami.Theme.colorSet: Kirigami.Theme.Complementary
        Kirigami.Theme.inherit: false

        icon.name: "view-refresh"
        icon.height: header.iconLength
        icon.width: header.iconLength
        icon.color: "white"

        visible: !Kirigami.Settings.isMobile
        text: i18n("Refresh")
        display: ToolButton.IconOnly
        onClicked: weatherLocation.update()

        ToolTip.visible: down
        ToolTip.text: i18n("Refresh")
    }
}
