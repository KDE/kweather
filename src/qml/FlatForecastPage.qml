/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami
import org.kde.kweather

SwipeView {
    id: forecastView
    property bool canGoLeft: currentIndex > 0
    property bool canGoRight: currentIndex < WeatherLocationListModel.count - 1

    anchors.fill: parent
    transform: Translate {
        y: yTranslate
    }

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
            model = Qt.binding(function () {
                return WeatherLocationListModel.locations;
            });
        }
        delegate: FlatLocationForecast {
            weatherLocation: modelData
        }
    }
}
