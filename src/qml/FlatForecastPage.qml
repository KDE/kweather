/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.12 as Kirigami

SwipeView {
    id: forecastView
    property bool canGoLeft: currentIndex > 0
    property bool canGoRight: currentIndex < weatherLocationListModel.count - 1
    
    anchors.fill: parent
    transform: Translate { y: yTranslate }
    opacity: mainItem.opacity
    
    function moveLeft() {
        currentIndex--;
    }
    function moveRight() {
        currentIndex++;
    }
    
    Repeater {
        id: forecastViewRepeater
        anchors.fill: parent
        model: weatherLocationListModel.locations
        delegate: FlatLocationForecast {
            weatherLocation: modelData
        }
    }
}
