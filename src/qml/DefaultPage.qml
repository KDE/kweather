/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.12 as Kirigami

// page shown if there are no weather locations configured
Kirigami.Page {
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
            iconName: "settings-configure"
            onTriggered: applicationWindow().pushPage(getPage("Settings"), 0)
        }
    ]

    Item { // empty list view to centre placeholdermessage
        anchors.fill: parent
        transform: Translate { y: yTranslate }
        BusyIndicator {
            anchors.centerIn: parent
            running: loading
            Layout.minimumWidth: Kirigami.Units.iconSizes.huge
            Layout.minimumHeight: width
        }
        
        
        ColumnLayout {
            visible: !loading
            anchors.centerIn: parent
            spacing: Kirigami.Units.gridUnit
            
            Kirigami.Icon {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                source: "qrc:/resources/kweather.svg"
                implicitWidth: Kirigami.Units.iconSizes.enormous * 1.5
                implicitHeight: Kirigami.Units.iconSizes.enormous * 1.5
            }
            
            Kirigami.Heading {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                text: i18n("Weather")
                type: Kirigami.Heading.Type.Primary
                horizontalAlignment: Qt.AlignHCenter
            }
            
            Button {
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                icon.name: "list-add"
                text: i18n("Add Location")
                onClicked: applicationWindow().openAddLocation();
            }
        }
    }
}
