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
    
    width: Kirigami.Units.gridUnit * 27
    height: Kirigami.Units.gridUnit * 45

    pageStack.initialPage: initPage()

    Component.onCompleted: {
        if (settingsModel.firstStartup) {
            let component = Qt.createComponent("qrc:/qml/SetupWizard.qml");
            let window = component.createObject(appwindow)
            window.show()
        }
    }
    
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
        title: i18n("Weather")
        bannerImageSource: "qrc:/resources/autumn.jpg"

        bannerVisible: true
        modal: !wideScreen

        actions: [
            Kirigami.Action {
                text: i18n("Forecast")
                iconName: "weather-clear"
                onTriggered: switchToPage(initPage());
            },
            Kirigami.Action {
                text: i18n("Locations")
                iconName: "globe"
                onTriggered: switchToPage(locationsPage);
            },
            Kirigami.Action {
                text: i18n("Settings")
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

    LocationsPage {
        id: locationsPage
        visible: false
    }

    SettingsPage {
        id: settingsPage
        visible: false
    }

    AddLocationPage {
        id: addLocationPage
        visible: false
    }

    Kirigami.AboutPage {
        id: aboutPage
        visible: false
        aboutData: {
            "displayName": i18n("Weather"),
            "productName": "kirigami/weather",
            "componentName": "kweather",
            "shortDescription": i18n("A mobile friendly weather app built with Kirigami."),
            "homepage": "",
            "bugAddress": "",
            "version": "0.2",
            "otherText": "",
            "copyrightStatement": i18n("© 2020 Plasma Development Team"),
            "desktopFileName": "org.kde.kweather",
            "authors": [
                {
                    "name": i18n("Han Young"),
                    "emailAddress": "hanyoung@protonmail.com",
                },
                {
                    "name": i18n("Devin Lin"),
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
