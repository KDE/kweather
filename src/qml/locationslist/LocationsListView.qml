/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020-2022 Devin Lin <espidev@gmail.com>
 * SPDX-FileCopyrightText: 2021 Nicolas Fella <nicolas.fella@gmx.de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml.Models

import org.kde.kirigami as Kirigami

import org.kde.kweather

import org.kde.kweather.components

ListView {
    id: root

    signal closeRequested

    DelegateModel {
        id: visualModel
        model: WeatherLocationListModel
        delegate: delegateComponent
    }

    model: visualModel

    reuseItems: true
    currentIndex: -1 // no default highlight

    add: Transition {
        NumberAnimation {
            property: "opacity"
            from: 0
            to: 1.0
            duration: Kirigami.Units.shortDuration
        }
    }
    remove: Transition {
        NumberAnimation {
            property: "opacity"
            from: 0
            to: 1.0
            duration: Kirigami.Units.shortDuration
        }
    }
    displaced: Transition {
        NumberAnimation {
            properties: "x,y"
            duration: Kirigami.Units.longDuration
            easing.type: Easing.InOutQuad
        }
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
            icon.name: "list-add"
            text: i18n("Add Location")
            onTriggered: applicationWindow().openAddLocation()
        }
    }

    Component {
        id: delegateComponent

        Item {
            id: dragParent
            width: listItem.width
            height: listItem.height

            property int visualIndex: DelegateModel.itemsIndex
            property bool held: false // whether it is being dragged
            z: held ? 1 : 0

            // logic for receiving drag events
            DropArea {
                anchors.fill: parent

                onEntered: drag => {
                    let from = drag.source.visualIndex;
                    let to = dragParent.visualIndex;
                    if (from !== undefined && to !== undefined && from !== to) {
                        visualModel.items.move(from, to);
                        WeatherLocationListModel.move(from, to);
                    }
                }
            }

            ListDelegate {
                id: listItem
                width: root.width

                leftPadding: Kirigami.Units.largeSpacing
                rightPadding: Kirigami.Units.largeSpacing
                topPadding: Kirigami.Units.largeSpacing
                bottomPadding: Kirigami.Units.largeSpacing

                onClicked: {
                    root.closeRequested();
                    applicationWindow().switchToPage(applicationWindow().getPage("Forecast"), 0);
                    applicationWindow().getPage("Forecast").switchPageIndex(index);
                }

                function deleteItem() {
                    // if there are no locations left
                    if (WeatherLocationListModel.count === 1) {
                        root.closeRequested();
                    }
                    WeatherLocationListModel.remove(index);
                }

                // drag logic
                Drag.active: dragParent.held
                Drag.source: dragHandle
                Drag.hotSpot.x: width / 2
                Drag.hotSpot.y: height / 2

                // remove anchors when dragging
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                states: [
                    State {
                        when: dragHandle.drag.active
                        ParentChange {
                            target: listItem
                            parent: root
                        }

                        AnchorChanges {
                            target: listItem
                            anchors.horizontalCenter: undefined
                            anchors.verticalCenter: undefined
                        }
                    }
                ]

                // list delegate contents
                contentItem: RowLayout {
                    spacing: 0

                    // handle for desktop users to drag
                    MouseArea {
                        id: dragHandle
                        implicitWidth: Kirigami.Units.iconSizes.smallMedium
                        implicitHeight: Kirigami.Units.iconSizes.smallMedium
                        cursorShape: pressed ? Qt.ClosedHandCursor : Qt.OpenHandCursor
                        preventStealing: true

                        property int visualIndex: dragParent.visualIndex

                        drag.target: dragParent.held ? listItem : undefined
                        drag.axis: Drag.YAxis

                        onPressed: dragParent.held = true
                        onReleased: dragParent.held = false

                        Kirigami.Icon {
                            anchors.fill: parent
                            source: "handle-sort"
                            opacity: dragHandle.pressed || (!Kirigami.Settings.tabletMode && dragHandle.hovered) ? 1 : 0.6
                        }
                    }

                    // weather icon and temperature
                    ColumnLayout {
                        Layout.leftMargin: Kirigami.Units.largeSpacing * 2
                        Layout.rightMargin: Kirigami.Units.largeSpacing * 2
                        spacing: Kirigami.Units.smallSpacing
                        Kirigami.Icon {
                            Layout.alignment: Qt.AlignHCenter
                            source: model.location.hourForecasts[0] ? model.location.hourForecasts[0].weatherIcon : "weather-none-available"
                            Layout.maximumHeight: Kirigami.Units.iconSizes.sizeForLabels * 2
                            Layout.preferredWidth: height
                            Layout.preferredHeight: Kirigami.Units.iconSizes.sizeForLabels * 2
                        }
                        Label {
                            Layout.alignment: Qt.AlignHCenter
                            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
                            text: Formatter.formatTemperatureRounded(model.location.hourForecasts[0].temperature, settingsModel.temperatureUnits)
                        }
                    }

                    // location name
                    Kirigami.Heading {
                        Layout.alignment: Qt.AlignLeft
                        Layout.fillWidth: true

                        level: 2
                        text: model.location.name
                        elide: Text.ElideRight
                        maximumLineCount: 2
                        wrapMode: Text.Wrap
                    }

                    ToolButton {
                        Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                        Layout.leftMargin: Kirigami.Units.smallSpacing

                        icon.name: "delete"
                        text: i18n("Delete")
                        onClicked: listItem.deleteItem()
                        display: AbstractButton.IconOnly

                        ToolTip.delay: Kirigami.Units.toolTipDelay
                        ToolTip.timeout: 5000
                        ToolTip.visible: Kirigami.Settings.tabletMode ? pressed : hovered
                        ToolTip.text: text
                    }
                }
            }
        }
    }

    delegate: delegateComponent
}
