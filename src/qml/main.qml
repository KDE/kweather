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
    
    width: Kirigami.Settings.isMobile ? Kirigami.Units.gridUnit * 27 : Kirigami.Units.gridUnit * 40
    height: Kirigami.Settings.isMobile ? Kirigami.Units.gridUnit * 45 : Kirigami.Units.gridUnit * 35

    Component.onCompleted: {
        switchToPage(getPage("Forecast"), 1); // initial page

        if (settingsModel.firstStartup) {
            setupWizardLoader.source = "qrc:/qml/SetupWizard.qml";
            setupWizardLoader.item.open();
        }
    }
    
    Loader {
        id: setupWizardLoader
    }
    
    Kirigami.PagePool {
        id: pagePool
    }
    
     // page switch animation
    NumberAnimation {
        id: anim
        from: 0
        to: 1
        duration: Kirigami.Units.longDuration * 2
        easing.type: Easing.InOutQuad
    }
    NumberAnimation {
        id: yAnim
        from: Kirigami.Units.gridUnit * 3
        to: 0
        duration: Kirigami.Units.longDuration * 3
        easing.type: Easing.OutQuint
    }
    
    function switchToPage(page, depth) {
        while (pageStack.depth > depth) pageStack.pop();
        while (pageStack.layers.depth > 1) pageStack.layers.pop();
        
        // page switch animation
        yAnim.target = page;
        yAnim.properties = "yTranslate";
        anim.target = page;
        anim.properties = "mainItem.opacity";
        if (page.header) {
            anim.properties += ",header.opacity";
        }
        yAnim.restart();
        anim.restart();
        
        pageStack.push(page);
    }
    
    function getPage(name) {
        switch (name) {
            case "Forecast": return pagePool.loadPage(weatherLocationListModel.count() === 0 ? "qrc:/qml/DefaultPage.qml" : "qrc:/qml/ForecastContainerPage.qml");
            case "Locations": return pagePool.loadPage("qrc:/qml/LocationsPage.qml");
            case "AddLocation": return pagePool.loadPage("qrc:/qml/AddLocationPage.qml");
            case "Settings": return pagePool.loadPage("qrc:/qml/SettingsPage.qml");
            case "About": return pagePool.loadPage("qrc:/qml/AboutPage.qml");
        }
    }

    globalDrawer: Kirigami.GlobalDrawer {
        title: i18n("Weather")
        titleIcon: "qrc:/resources/kweather.svg"
        
        property bool isWidescreen: appwindow.width > appwindow.height
        
        bannerVisible: true
        modal: !isWidescreen
        collapsible: true
        collapsed: isWidescreen
        width: collapsed ? implicitWidth : Kirigami.Units.gridUnit * 12

        Behavior on width {
            NumberAnimation {
                duration: Kirigami.Units.longDuration
                easing.type: Easing.InOutQuad
            }
        }
        
        actions: [
            Kirigami.Action {
                text: i18n("Forecast")
                iconName: "weather-clear"
                onTriggered: switchToPage(getPage("Forecast"), 0);
            },
            Kirigami.Action {
                text: i18n("Locations")
                iconName: "globe"
                onTriggered: switchToPage(getPage("Locations"), 0);
            },
            Kirigami.Action {
                text: i18n("Settings")
                iconName: "settings-configure"
                onTriggered: switchToPage(getPage("Settings"), 0);
            }
        ]
    }
    FontLoader {
        id: lightHeadingFont
        source: "/resources/NotoSans-Light.ttf"
    }
}
