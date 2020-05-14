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
    title: forecastView.count == 0 ? "Forecast" : weatherLocationListModel.get(forecastView.currentIndex).name

    Kirigami.PlaceholderMessage {
        anchors.centerIn: parent
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Kirigami.Units.largeSpacing

        icon.name: "globe"
        text: i18n("No locations configured")
        visible: forecastView.count == 0

        helpfulAction: Kirigami.Action {
            iconName: "list-add"
            text: i18n("Add current location")
            onTriggered: weatherLocationListModel.requestCurrentLocation()
        }
    }

    SwipeView {
        id: forecastView
        anchors.fill: parent

        Repeater {
            id: forecastViewRepeater
            model: weatherLocationListModel
            anchors.fill: parent

            CityForecast {
                weatherLocation: weatherLocationListModel.get(index)
            }
        }
    }
    PageIndicator {
        z: 10
        id: indicator

        count: forecastView.count
        currentIndex: forecastView.currentIndex

        anchors.bottom: forecastView.bottom
        anchors.horizontalCenter: forecastView.horizontalCenter
    }
}


