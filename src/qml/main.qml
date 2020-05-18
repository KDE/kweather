/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.11 as Kirigami

Kirigami.ApplicationWindow
{
    id: appwindow
    title: i18n("Weather")
    visible: true
    
    width: Kirigami.Units.gridUnit * 27
    height: Kirigami.Units.gridUnit * 45

    pageStack.initialPage: initPage()

    function switchToPage(page) {
        while (pageStack.depth > 0) pageStack.pop();
        pageStack.push(page);
    }

    function initPage() {
        if(weatherLocationListModel.count() === 0)
            return defaultPage;
        else
            return forecastPage;
    }

    globalDrawer: Kirigami.GlobalDrawer {
        title: "Weather"
        bannerImageSource: "qrc:/resources/autumn.jpg"

        bannerVisible: true
//         modal: !wideScreen
//         width: 200

        actions: [
            Kirigami.Action {
                text: "Forecast"
                iconName: "weather-clear"
                onTriggered: switchToPage(initPage());
            },
            Kirigami.Action {
                text: "Locations"
                iconName: "globe"
                onTriggered: switchToPage(citiesPage);
            },
            Kirigami.Action {
                text: "Settings"
                iconName: "settings-configure"
                onTriggered: switchToPage(settingsPage);
            }
        ]
    }

    DefaultPage {
        id: defaultPage
    }

    ForecastContainerPage {
        id: forecastPage
    }

    CitiesPage {
        id: citiesPage
        visible: false
    }

    SettingsPage {
        id: settingsPage
        visible: false
    }

    AddCityPage {
        id: addCityPage
        visible: false
    }

    Kirigami.AboutPage {
        id: aboutPage
        visible: false
        aboutData: {
            "displayName": "Weather",
            "productName": "kirigami/weather",
            "componentName": "kweather",
            "shortDescription": "A mobile friendly weather app built with Kirigami.",
            "homepage": "",
            "bugAddress": "",
            "version": "0.1",
            "otherText": "",
            "copyrightStatement": "© 2020 Plasma Development Team",
            "desktopFileName": "org.kde.kweather",
            "authors": [
                {
                    "name": "Han Young",
                    "emailAddress": "hanyoung@protonmail.com",
                },
                {
                    "name": "Devin Lin",
                    "emailAddress": "espidev@gmail.com",
                    "webAddress": "https://espi.dev"
                }
            ],
            "licenses": [
                {
                    "name": "GPL v2",
                    "text": "long, boring, license text",
                    "spdx": "GPL-v2.0",
                }
            ]
        }
    }

}
