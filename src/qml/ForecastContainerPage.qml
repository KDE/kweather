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

    // left right arrows for desktop
    actions.contextualActions: [
        Kirigami.Action {
                iconName: "arrow-left"
                onTriggered: forecastView.currentIndex--
                enabled: forecastView.currentIndex > 0
        },
        Kirigami.Action {
            iconName: "arrow-right"
            onTriggered: forecastView.currentIndex++
            enabled: forecastView.currentIndex < forecastView.count-1
        }
    ]

    // individual locations
    SwipeView {
        id: forecastView
        anchors.fill: parent

        Repeater {
            id: forecastViewRepeater
            model: weatherLocationListModel
            anchors.fill: parent

            Loader {
                id: locationLoader
                property bool inView: SwipeView.isCurrentItem
                onInViewChanged: {
                    locationLoader.item["inView"] = inView;
                }

                Component.onCompleted: init()

                function init(){
                    updateForecastStyle();
                    locationLoader.item["inView"] = locationLoader.inView;
                }

                function updateForecastStyle() {
                    if (settingsModel.forecastStyle === "Dynamic") {
                        console.log("loaded dynamic view");
                        locationLoader.setSource("LocationForecast2.qml", {"weatherLocation": location});
                    } else { // "Flat"
                        console.log("loaded flat view");
                        locationLoader.setSource("LocationForecast.qml", {"weatherLocation": location});
                    }
                }
                
                Connections {
                    target: settingsModel
                    function onForecastStyleChanged() {
                        updateForecastStyle();
                    }
                }
            }
        }
    }

    // TODO figure out why pageindicator needs to be here in order for swipeview to display
    PageIndicator {
        anchors.horizontalCenter: page.horizontalCenter
        visible: forecastView.count > 1
        count: forecastView.count
        currentIndex: forecastView.currentIndex
    }
}


