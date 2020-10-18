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
    property bool inView: false
    gradient: Gradient {
        GradientStop { color: "#455a64"; position: 0.0 }
        GradientStop { color: "#263238"; position: 1.0 }
    }

    Stars {}
    
    Kirigami.Icon {
        opacity: 0.7
        width: 100
        height: 100
        x: 20
        y: 40
        source: "weather-clear-night"
    }
    
    Cloudy {
        cloudColor: "#b0bec5"
        inView: parent.inView
    }
}
