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

Kirigami.ScrollablePage {
    property alias pageIndex: forecastView.currentIndex

    id: page
    title: forecastView.count == 0 ? i18n("Forecast") : weatherLocationListModel.get(forecastView.currentIndex).name

    SwipeView {
        id: forecastView
        anchors.fill: parent

        Repeater {
            id: forecastViewRepeater
            model: weatherLocationListModel
            anchors.fill: parent

            LocationForecast {
                weatherLocation: weatherLocationListModel.get(index)
            }
        }
    }

    // TODO figure out why pageindicator needs to be here in order for swipeview to display
    PageIndicator {
        visible: forecastView.count > 1
        count: forecastView.count
        currentIndex: forecastView.currentIndex
    }
}


