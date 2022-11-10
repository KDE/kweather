/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2022 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.15
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.12 as Kirigami

import kweather 1.0

import "../components"

ListView {
    id: root
    model: WeatherLocationListModel.locations
    
    property bool addPadding: false
    signal closeRequested()
    
    reuseItems: true
    currentIndex: -1 // no default highlight

    add: Transition {
        NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: Kirigami.Units.shortDuration }
    }
    remove: Transition {
        NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: Kirigami.Units.shortDuration }
    }
    displaced: Transition {
        NumberAnimation { properties: "x,y"; duration: Kirigami.Units.longDuration; easing.type: Easing.InOutQuad}
    }
    moveDisplaced: Transition {
        YAnimator {
            duration: Kirigami.Units.longDuration
            easing.type: Easing.InOutQuad
        }
    }
    
    Connections {
        target: WeatherLocationListModel
        function onNetworkErrorCreating() {
            showPassiveNotification(i18n("Unable to fetch timezone information"));
        }
    }

    Kirigami.PlaceholderMessage {
        anchors.centerIn: parent
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: Kirigami.Units.largeSpacing

        icon.name: "globe"
        text: i18n("Add a location")
        visible: root.count == 0
        
        helpfulAction: Kirigami.Action {
            iconName: "list-add"
            text: i18n("Add Location")
            onTriggered: applicationWindow().openAddLocation()
        }
    }

    delegate: ListDelegate {
        id: listItem
        width: root.width
        showSeparator: model.index != root.count - 1
        
        leftPadding: Kirigami.Units.largeSpacing * (root.addPadding ? 2 : 1)
        rightPadding: Kirigami.Units.largeSpacing * (root.addPadding ? 2 : 1)
        topPadding: Kirigami.Units.largeSpacing
        bottomPadding: Kirigami.Units.largeSpacing
        
        function deleteItem() {
            // if there are no locations left
            if (WeatherLocationListModel.count === 1) {
                root.closeRequested();
            }
            WeatherLocationListModel.remove(index);
        }
        
        onClicked: {
            root.closeRequested();
            applicationWindow().switchToPage(applicationWindow().getPage("Forecast"), 0);
            applicationWindow().getPage("Forecast").switchPageIndex(index);
        }

        contentItem: RowLayout {
            spacing: Kirigami.Units.largeSpacing * 2

            Kirigami.ListItemDragHandle {
                listItem: listItem
                listView: root
                onMoveRequested: {
                    WeatherLocationListModel.move(oldIndex, newIndex);
                    root.currentIndex = -1;
                }
                onDropped: {
                    root.currentIndex = -1;
                }
            }
            
            ColumnLayout {
                spacing: Kirigami.Units.smallSpacing
                Kirigami.Icon {
                    Layout.alignment: Qt.AlignHCenter
                    source: (modelData && modelData.hourForecasts[0]) ? modelData.hourForecasts[0].weatherIcon : "weather-none-available"
                    Layout.maximumHeight: Kirigami.Units.iconSizes.sizeForLabels * 2
                    Layout.preferredWidth: height
                    Layout.preferredHeight: Kirigami.Units.iconSizes.sizeForLabels * 2
                }
                Label {
                    Layout.alignment: Qt.AlignHCenter
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
                    text: modelData ? Formatter.formatTemperatureRounded(modelData.hourForecasts[0].temperature, settingsModel.temperatureUnits) : ""
                }
            }

            Kirigami.Heading {
                Layout.alignment: Qt.AlignLeft
                Layout.fillWidth: true
                
                level: 2
                text: modelData ? modelData.name : ""
                elide: Text.ElideRight
                maximumLineCount: 2
                wrapMode: Text.Wrap
            }
            
            ToolButton {
                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                icon.name: "delete"
                text: i18n("Delete")
                onClicked: listItem.deleteItem();
                //visible: root.editMode
                display: AbstractButton.IconOnly
                
                ToolTip.delay: Kirigami.Units.toolTipDelay
                ToolTip.timeout: 5000
                ToolTip.visible: Kirigami.Settings.tabletMode ? pressed : hovered
                ToolTip.text: text
            }
        }
    }
}
