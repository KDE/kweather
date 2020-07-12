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
    title: i18n("Add location")

    property string searchQuery: ""

    ListView {
        id: addCityList
        header: Item {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Kirigami.Units.largeSpacing * 2
            anchors.rightMargin: Kirigami.Units.largeSpacing * 2
            height: search.height * 1.5

            Row {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.topMargin: Kirigami.Units.largeSpacing
                Layout.fillWidth: true

                TextField {
                    id: search
                    anchors.right: sourceButton.left
                    anchors.left: parent.left

                    placeholderText: i18n("Search for a place...")
                    onTextChanged: {
                        searchQuery = text
                        locationQueryModel.textChanged(text)
                    }
                    onEditingFinished: locationQueryModel.textChanged(text, 0) // when return is pressed, query immediately
                }
                Button {
                    id: sourceButton
                    icon.name: "settings-configure"
                    width: height
                    anchors.right: searchButton.left
                    onClicked: apiSelector.open()
                }
                Button {
                    id: searchButton
                    icon.name: "search"
                    width: height
                    anchors.right: parent.right
                    onClicked: locationQueryModel.textChanged(searchQuery, 0)
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
            visible: locationQueryModel.networkError
        }

        // default message (has not searched yet)
        Kirigami.PlaceholderMessage {
            anchors.centerIn: parent
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: Kirigami.Units.largeSpacing

            icon.name: "search"
            text: i18n("Search for a location")
            visible: !locationQueryModel.networkError && !locationQueryModel.loading && addCityList.count == 0 && searchQuery == ""

            helpfulAction: Kirigami.Action {
                iconName: "list-add"
                text: i18n("Add current location")
                onTriggered: {
                    weatherLocationListModel.requestCurrentLocation()
                    defaultPage.loading = true
                    switchToPage(defaultPage)
                }
            }
        }

        // no results
        Kirigami.PlaceholderMessage {
            anchors.centerIn: parent
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: Kirigami.Units.largeSpacing

            icon.name: "search"
            text: i18n("No results")
            visible: !locationQueryModel.networkError && !locationQueryModel.loading && addCityList.count == 0 && searchQuery != ""
        }

        // loading results indicator
        BusyIndicator {
            anchors.centerIn: parent
            running: locationQueryModel.loading
            Layout.minimumWidth: Kirigami.Units.iconSizes.huge
            Layout.minimumHeight: width
        }

        anchors.fill: parent
        model: locationQueryModel
        delegate: Kirigami.SwipeListItem {
            Label {
                text: model.name
            }
            onClicked: {
                locationQueryModel.addLocation(index);
                switchToPage(locationsPage);
            }
        }
    }

    // api source selector
    Kirigami.OverlaySheet {
        id: apiSelector
        header: Label {
            text: i18n("Select Weather Source")
        }

        ListView {
            Layout.leftMargin: Kirigami.Units.gridUnit
            Layout.preferredWidth: Kirigami.Units.gridUnit * 25
            model: ListModel {
                ListElement {
                    display: "api.met.no (Norwegian Weather Service)"
                    name: "nmiweatherapi"
                }
                ListElement {
                    display: "api.openweathermap.org (OpenWeather)"
                    name: "owmweatherapi"
                }
            }

            delegate: RadioDelegate {
                width: parent.width
                text: model.display
                checked: settingsModel.defaultBackend == model.name
                onCheckedChanged: {
                    if (checked) {
                        settingsModel.defaultBackend = model.name;
                        settingsModel.save();
                    }
                }
            }
        }
    }
}
