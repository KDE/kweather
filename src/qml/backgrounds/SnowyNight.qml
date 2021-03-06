/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */
import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtQuick.Shapes 1.12
import org.kde.kirigami 2.11 as Kirigami
import "components"

Rectangle {
    property bool inView: true
    gradient: Gradient {
        GradientStop { color: "#455a64"; position: 0.0 }
        GradientStop { color: "#263238"; position: 1.0 }
    }

    Snow {
        anchors.fill: parent
        inView: parent.inView
    }
}
