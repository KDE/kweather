/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2022 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

import org.kde.kweather

import org.kde.kweather.components

ListView {
    id: root

    property string searchQuery: ""

    model: LocationQueryModel {}

    signal closeRequested
    signal focusRequested

    header: Control {
        width: root.width
        leftPadding: Kirigami.Units.largeSpacing
        rightPadding: Kirigami.Units.largeSpacing
        topPadding: Kirigami.Units.largeSpacing
        bottomPadding: Kirigami.Units.largeSpacing

        contentItem: RowLayout {
            spacing: Kirigami.Units.smallSpacing

            Kirigami.SearchField {
                id: search
                Layout.fillWidth: true

                Connections {
                    target: root
                    function onFocusRequested() {
                        search.forceActiveFocus();
                    }
                }

                placeholderText: i18n("Search for a location…")
                onTextChanged: {
                    root.searchQuery = text;
                    root.model.textChanged(text);
                    root.currentIndex = 0;
                }
                onEditingFinished: {
                    if (root.searchQuery === text) {
                        // no change
                        return;
                    }
                    root.searchQuery = text;
                    root.model.textChanged(text, 0); // when return is pressed, query immediately
                    root.currentIndex = 0;
                }
                KeyNavigation.down: root
            }

            Button {
                id: searchButton
                icon.name: "search"
                implicitHeight: search.implicitHeight
                implicitWidth: height
                enabled: search.text !== ""
                width: height
                height: search.height
                onClicked: root.model.textChanged(root.searchQuery, 0)
            }
        }
    }

    // unable to connect message
    Kirigami.PlaceholderMessage {
        anchors.centerIn: parent
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Kirigami.Units.largeSpacing

        icon.name: "network-disconnect"
        text: i18n("Unable to connect")
        visible: root.model.networkError
    }

    // default message (has not searched yet)
    Kirigami.PlaceholderMessage {
        anchors.centerIn: parent
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Kirigami.Units.largeSpacing

        icon.name: "search"
        text: i18n("Search for a location")
        visible: !root.model.networkError && !root.model.loading && root.count == 0 && root.searchQuery == ""

        // The Mozilla location service has shutdown, and so in many cases geoclue stops working and hangs...
        // Disable this for now until we have a good way to use GPS or other geolocation services.
        //
        // helpfulAction: Kirigami.Action {
        //     icon.name: "mark-location"
        //     text: i18n("Add current location")
        //     onTriggered: {
        //         WeatherLocationListModel.requestCurrentLocation();
        //         root.closeRequested();
        //         let page = getPage("Forecast");
        //         switchToPage(page, 0);
        //         if (page.loading !== undefined) {
        //             getPage("Forecast").loading = true;
        //         }
        //     }
        // }
    }

    // no results
    Kirigami.PlaceholderMessage {
        anchors.centerIn: parent
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Kirigami.Units.largeSpacing

        icon.name: "search"
        text: i18n("No results")
        visible: !root.model.networkError && !root.model.loading && root.count == 0 && root.searchQuery != ""
    }

    // loading results indicator
    BusyIndicator {
        anchors.centerIn: parent
        running: root.model.loading && root.count === 0
        Layout.minimumWidth: Kirigami.Units.iconSizes.huge
        Layout.minimumHeight: width
    }

    delegate: ListDelegate {
        width: root.width
        showSeparator: model.index != root.count - 1

        leftPadding: Kirigami.Units.largeSpacing
        rightPadding: Kirigami.Units.largeSpacing
        topPadding: Kirigami.Units.largeSpacing + Kirigami.Units.smallSpacing * 2
        bottomPadding: Kirigami.Units.largeSpacing + Kirigami.Units.smallSpacing * 2

        function apply() {
            root.model.addLocation(index);
            applicationWindow().getPage("Forecast").switchPageIndex(WeatherLocationListModel.count - 1);
            switchToPage(appwindow.getPage("Forecast"), 0);
            root.closeRequested();
        }

        onClicked: apply()
        Keys.onPressed: event => {
            if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
                apply();
                event.accepted = true;
            } else {
                event.accepted = false;
            }
        }

        contentItem: RowLayout {
            spacing: Kirigami.Units.largeSpacing

            Label {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
                text: model.name
            }
        }
    }
}
