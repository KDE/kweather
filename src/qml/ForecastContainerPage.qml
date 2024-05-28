/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2021 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.kde.kirigami as Kirigami

import org.kde.kweather

Kirigami.Page {
    id: page
    topPadding: 0
    bottomPadding: 0
    rightPadding: 0
    leftPadding: 0

    title: {
        if (WeatherLocationListModel.locations.count == 0) {
            return i18n("Forecast");
        } else if (settingsModel.forecastStyle === "Dynamic") {
            return ""; // provided by DynamicForecastPage
        } else if (WeatherLocationListModel.locations[loader.item.currentIndex]) {
            return WeatherLocationListModel.locations[loader.item.currentIndex].name;
        } else {
            return "";
        }
    }

    globalToolBarStyle: (settingsModel.forecastStyle === "Dynamic" && pageStack.layers.depth <= 1) ? Kirigami.ApplicationHeaderStyle.None : Kirigami.ApplicationHeaderStyle.ToolBar

    property real yTranslate: 0

    function switchPageIndex(pageIndex) {
        loader.item.currentIndex = pageIndex;
    }

    // actions (only shown in flat view since the toolbar is hidden in dynamic view)
    actions: [
        Kirigami.Action {
            icon.name: "find-location"
            text: i18n("Locations")
            onTriggered: applicationWindow().openLocationsList()
        },
        Kirigami.Action {
            icon.name: "settings-configure"
            text: i18n("Settings")
            displayHint: Kirigami.Action.IconOnly
            onTriggered: applicationWindow().openSettings()
        },
        Kirigami.Action {
            visible: !Kirigami.Settings.isMobile
            icon.name: "view-refresh"
            text: i18n("Refresh")
            displayHint: Kirigami.Action.IconOnly
            onTriggered: WeatherLocationListModel.locations[loader.item.currentIndex].update()
        }
    ]

    Loader {
        id: loader
        transform: Translate {
            y: yTranslate
        }
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
