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

import org.kde.kweather.locations
import org.kde.kweather.settings

import org.kde.kweather

Kirigami.ApplicationWindow {
    id: appwindow
    title: i18n("Weather")

    minimumWidth: Kirigami.Settings.isMobile ? 0 : 360
    minimumHeight: Kirigami.Settings.isMobile ? 0 : 360
    width: Kirigami.Settings.isMobile ? Kirigami.Units.gridUnit * 27 : Kirigami.Units.gridUnit * 40
    height: Kirigami.Settings.isMobile ? Kirigami.Units.gridUnit * 45 : Kirigami.Units.gridUnit * 35

    pageStack.globalToolBar.canContainHandles: true // move handles to toolbar
    pageStack.globalToolBar.style: Kirigami.ApplicationHeaderStyle.ToolBar // ensure toolbar style for mobile
    pageStack.globalToolBar.showNavigationButtons: Kirigami.ApplicationHeaderStyle.ShowBackButton
    pageStack.popHiddenPages: true

    pageStack.columnView.columnResizeMode: Kirigami.ColumnView.SingleColumn

    readonly property bool isWideScreen: width > 540

    readonly property bool isDialogOpen: settingsDialogLoader.isOpen || addLocationDialogLoader.isOpen || locationsListDialogLoader.isOpen

    Component.onCompleted: {
        // initial page
        switchToPage(getPage("Forecast"), 1);
        if (settingsModel.firstStartup) {
            setupWizardLoader.source = "SetupWizard.qml";
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
        while (pageStack.layers.depth > 1) {
            pageStack.layers.pop();
        }
        while (pageStack.currentItem !== page && pageStack.depth > depth) {
            pageStack.pop();
        }

        if (pageStack.currentItem !== page) {
            runPageAnimations(page);
            pageStack.push(page);
        }
    }

    function pushPage(page, depth) {
        while (depth !== undefined && pageStack.depth > depth + 1)
            pageStack.pop();
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
        console.log('fetch ' + name);
        switch (name) {
        case "Forecast":
            return pagePool.loadPage(WeatherLocationListModel.count === 0 ? getPageUrl('', 'DefaultPage.qml') : getPageUrl('', 'ForecastContainerPage.qml'));
        case "Locations":
            return pagePool.loadPage("qrc:/qt/qml/org/kde/kweather/locations/qml/locationslist/LocationsListPage.qml");
        case "Settings":
            return pagePool.loadPage(getPageUrl('settings', 'settings/SettingsPage.qml'));
        case "About":
            return pagePool.loadPage(getPageUrl('', 'AboutPage.qml'));
        case "Default":
            return pagePool.loadPage(getPageUrl('', 'DefaultPage.qml'));
        }
    }

    function getPageUrl(module: string, file: string): string {
        if (module === '') {
            return `qrc:/qt/qml/org/kde/kweather/qml/${file}`;
        }
        return `qrc:/qt/qml/org/kde/kweather/${module}/qml/${file}`;
    }

    function openSettings() {
        if (isWideScreen) {
            settingsDialogLoader.active = true;
            if (Kirigami.Settings.isMobile) {
                // SettingsDialog
                settingsDialogLoader.item.open();
            } else {
                // SettingsWindow
                settingsDialogLoader.item.close();
                settingsDialogLoader.item.show();
            }
        } else {
            applicationWindow().pushPage(getPage("Settings"), 0);
        }
    }

    function openLocationsList() {
        if (isWideScreen) {
            locationsListDialogLoader.active = true;
            locationsListDialogLoader.item.open();
        } else {
            applicationWindow().pushPage(getPage("Locations"), 0);
        }
    }

    function openAddLocation() {
        if (isWideScreen) {
            addLocationDialogLoader.active = true;
            addLocationDialogLoader.item.open();
        } else {
            applicationWindow().pageStack.push(getPageUrl('locations', 'locationslist/AddLocationPage.qml'));
        }
    }

    Connections {
        target: WeatherLocationListModel

        function onLocationsChanged() {
            // wwitch to default page if the count is zero
            if (WeatherLocationListModel.count === 0) {
                applicationWindow().switchToPage(applicationWindow().getPage("Default"), 0);
            }
        }
    }

    Loader {
        id: settingsDialogLoader
        property bool isOpen: item && item.visible
        active: false
        sourceComponent: Kirigami.Settings.isMobile ? Qt.createComponent("org.kde.kweather.settings", "SettingsDialog") : Qt.createComponent("org.kde.kweather.settings", "SettingsWindow")
    }

    Loader {
        id: addLocationDialogLoader
        property bool isOpen: item && item.visible
        active: false
        sourceComponent: AddLocationDialog {}
    }

    Loader {
        id: locationsListDialogLoader
        property bool isOpen: item && item.visible
        active: false
        sourceComponent: LocationsListDialog {}
    }

    FontLoader {
        id: lightHeadingFont
        source: "qrc:/resources/NotoSans-Light.ttf"
    }
}
