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

import kweather 1.0

SwipeView {
    id: forecastView
    property bool canGoLeft: currentIndex > 0
    property bool canGoRight: currentIndex < WeatherLocationListModel.count - 1
    
    anchors.fill: parent
    transform: Translate { y: yTranslate }
    
    function moveLeft() {
        currentIndex--;
    }
    function moveRight() {
        currentIndex++;
    }
    
    Repeater {
        id: forecastViewRepeater
        anchors.fill: parent
        
        // on mobile mode, for some reason, switching the type to dynamic and back to flat again gives an empty page unless we assign the model
        // after component creation
        Component.onCompleted: {
            model = Qt.binding(function() { return WeatherLocationListModel.locations; });
        }
        delegate: FlatLocationForecast {
            weatherLocation: modelData
        }
    }
}
