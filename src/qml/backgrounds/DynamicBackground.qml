/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Shapes
import org.kde.kirigami as Kirigami
import org.kde.kweather.backgrounds.components

// parent of all weather background components
Rectangle {
    color: "transparent"
    anchors.fill: parent
    opacity: 0 // animated intro

    // specify background colour
    property color gradientColorTop: "white"
    property color gradientColorBottom: "white"

    property bool sun: false

    property bool clouds: false
    property color cloudsColor: "white"

    property bool stars: false

    property bool rain: false

    property bool snow: false
}
