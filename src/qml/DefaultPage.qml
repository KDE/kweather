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

// page shown if there are no weather locations configured
Kirigami.ScrollablePage {
    title: i18n("Forecast")

    property bool loading: false
    
    property int yTranslate: 0
    
    Connections {
        target: weatherLocationListModel
        function onNetworkErrorCreatingDefault() {
            showPassiveNotification(i18n("Network error when obtaining current location"));
            loading = false;
        }
        function onSuccessfullyCreatedDefault() {
            switchToPage(getPage("Forecast"), 0);
            loading = false;
        }
    }
    
    actions.contextualActions: [
        Kirigami.Action {
            visible: Kirigami.Settings.isMobile
            iconName: "globe"
            onTriggered: applicationWindow().openLocationsList();
        },
        Kirigami.Action {
            visible: Kirigami.Settings.isMobile
            iconName: "settings-configure"
            onTriggered: applicationWindow().pushPage(getPage("Settings"), 0)
        }
    ]

    ListView { // empty list view to centre placeholdermessage
        id: listView
        transform: Translate { y: yTranslate }
        BusyIndicator {
            anchors.centerIn: parent
            running: loading
            Layout.minimumWidth: Kirigami.Units.iconSizes.huge
            Layout.minimumHeight: width
        }
        Kirigami.PlaceholderMessage {
            visible: !loading
            anchors.centerIn: parent
            anchors.margins: Kirigami.Units.largeSpacing

            icon.name: "globe"
            text: i18n("No locations configured")

            helpfulAction: Kirigami.Action {
                iconName: "list-add"
                text: i18n("Add Location")
                onTriggered: applicationWindow().openAddLocation();
            }
        }
    }
}
