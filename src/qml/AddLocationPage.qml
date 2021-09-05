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

import kweather 1.0

Kirigami.ScrollablePage {
    id: root

    title: i18n("Add location")

    property string searchQuery: ""

    property var model: LocationQueryModel {}

    header: RowLayout {
        anchors.margins: Kirigami.Units.largeSpacing
        spacing: Kirigami.Units.smallSpacing
        TextField {
            id: search
            Layout.fillWidth: true
            Layout.leftMargin: Kirigami.Units.largeSpacing

            placeholderText: i18n("Search for a place...")
            onTextChanged: {
                searchQuery = text
                root.model.textChanged(text)
            }
            onEditingFinished: root.model.textChanged(text, 0) // when return is pressed, query immediately
        }
        Button {
            id: searchButton
            implicitHeight: search.implicitHeight
            implicitWidth: height
            Layout.rightMargin: Kirigami.Units.largeSpacing
            icon.name: "search"
            width: height
            height: search.height
            onClicked: root.model.textChanged(searchQuery, 0)
        }
    }
    
    ListView {
        id: addCityList

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
            visible: !root.model.networkError && !root.model.loading && addCityList.count == 0 && searchQuery == ""

            helpfulAction: Kirigami.Action {
                iconName: "list-add"
                text: i18n("Add current location")
                onTriggered: {
                    weatherLocationListModel.requestCurrentLocation()
                    let page = getPage("Forecast");
                    switchToPage(page, 0);
                    if (page.loading !== undefined) {
                        getPage("Forecast").loading = true;
                    }
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
            visible: !root.model.networkError && !root.model.loading && addCityList.count == 0 && searchQuery != ""
        }

        // loading results indicator
        BusyIndicator {
            anchors.centerIn: parent
            running: root.model.loading && addCityList.count === 0
            Layout.minimumWidth: Kirigami.Units.iconSizes.huge
            Layout.minimumHeight: width
        }

        anchors.fill: parent
        model: root.model
        delegate: Kirigami.SwipeListItem {
            Label {
                text: model.name
            }
            onClicked: {
                root.model.addLocation(index);
                switchToPage(getPage("Locations"), 0);
            }
        }
    }
}
