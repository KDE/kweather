/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2021 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.11 as Kirigami

Kirigami.ApplicationWindow {
    id: appwindow
    title: i18n("Weather")
    
    minimumWidth: Kirigami.Settings.isMobile ? 0 : 360
    minimumHeight: Kirigami.Settings.isMobile ? 0 : 360
    width: Kirigami.Settings.isMobile ? Kirigami.Units.gridUnit * 27 : Kirigami.Units.gridUnit * 40
    height: Kirigami.Settings.isMobile ? Kirigami.Units.gridUnit * 45 : Kirigami.Units.gridUnit * 35
    
    pageStack.globalToolBar.canContainHandles: true; // move handles to toolbar
    pageStack.globalToolBar.style: Kirigami.ApplicationHeaderStyle.ToolBar; // ensure toolbar style for mobile
    pageStack.globalToolBar.showNavigationButtons: Kirigami.ApplicationHeaderStyle.ShowBackButton;

    pageStack.columnView.columnResizeMode: Kirigami.ColumnView.SingleColumn
    
    Component.onCompleted: {
        // initial page
        switchToPage(getPage("Forecast"), 1);

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
        while (pageStack.layers.depth > 1) pageStack.layers.pop();
        while (pageStack.depth > depth) pageStack.pop();
        runPageAnimations(page);
        pageStack.push(page);
    }
    
    function pushPage(page, depth) {
        while (depth !== undefined && pageStack.depth > depth + 1) pageStack.pop();
        runPageAnimations(page);
        pageStack.push(page);
    }
    
    function runPageAnimations(page) {
        yAnim.target = page;
        yAnim.properties = "yTranslate";
        anim.target = page;
        anim.properties = "contentItem.opacity";
        if (page.header) {
            anim.properties += ",header.opacity";
        }
        yAnim.restart();
        anim.restart();
    }
    
    function getPage(name) {
        switch (name) {
            case "Forecast": return pagePool.loadPage(weatherLocationListModel.count === 0 ? "qrc:/qml/DefaultPage.qml" : "qrc:/qml/ForecastContainerPage.qml");
            case "Locations": return pagePool.loadPage("qrc:/qml/LocationsPage.qml");
            case "AddLocation": return pagePool.loadPage("qrc:/qml/AddLocationPage.qml");
            case "Settings": return pagePool.loadPage("qrc:/qml/SettingsPage.qml");
            case "About": return pagePool.loadPage("qrc:/qml/AboutPage.qml");
        }
    }

    FontLoader {
        id: lightHeadingFont
        source: "qrc:/resources/NotoSans-Light.ttf"
    }
}
