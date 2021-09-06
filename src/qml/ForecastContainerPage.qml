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

Kirigami.Page {
    id: page
    topPadding: 0; bottomPadding: 0; rightPadding: 0; leftPadding: 0

    title: weatherLocationListModel.locations.count == 0 ? i18n("Forecast") : weatherLocationListModel.locations[loader.item.currentIndex].name
    
    property int yTranslate: 0
    
    function switchPageIndex(pageIndex) {
        loader.item.currentIndex = pageIndex;
    }
    
    // desktop actions
    actions.contextualActions: [
        Kirigami.Action {
            iconName: "view-refresh"
            onTriggered: weatherLocationListModel.locations[loader.item.currentIndex].update()
        },
        Kirigami.Action {
            iconName: "arrow-left"
            onTriggered: loader.item.moveLeft()
            enabled: loader.item.canGoLeft
        },
        Kirigami.Action {
            iconName: "arrow-right"
            onTriggered: loader.item.moveRight()
            enabled: loader.item.canGoRight
        }
    ]
    
    Loader {
        id: loader
        transform: Translate { y: yTranslate }
        anchors.fill: parent
        
        Component.onCompleted: loadStyle()
        function loadStyle() {
            setSource(settingsModel.forecastStyle === "Dynamic" ? "DynamicForecastPage.qml" : "FlatForecastPage.qml");
        }
        
        Connections {
            target: settingsModel
            function onForecastStyleChanged() {
                loader.loadStyle();
            }
        }
    }
}


