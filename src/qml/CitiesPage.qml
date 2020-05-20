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
import kweather 1.0

Kirigami.ScrollablePage {
    title: i18n("Locations")
    
    actions.main: Kirigami.Action {
        iconName: "list-add"
        text: i18n("Add Location")
        onTriggered: pageStack.push(addCityPage)
    }
    
    ListView {
        id: citiesList
        model: weatherLocationListModel

        moveDisplaced: Transition {
            YAnimator {
                duration: Kirigami.Units.longDuration
                easing.type: Easing.InOutQuad
            }
        }

        Kirigami.PlaceholderMessage {
            anchors.centerIn: parent
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: Kirigami.Units.largeSpacing

            icon.name: "globe"
            text: i18n("Add a location")
            visible: citiesList.count == 0
        }

        delegate: Kirigami.SwipeListItem {

            property WeatherLocation location: weatherLocationListModel.get(index)

            id: listItem
            actions: Kirigami.Action {
                iconName: "delete"
                text: i18n("Remove")
                onTriggered: {
                    weatherLocationListModel.remove(index);
                }
            }

            onClicked: {
                switchToPage(forecastPage);
                forecastPage.pageIndex = index;
            }

            contentItem: Item {
                implicitWidth: delegateLayout.implicitWidth
                implicitHeight: delegateLayout.implicitHeight
                RowLayout {
                    Layout.alignment: Qt.AlignLeft
                    id: delegateLayout
                    anchors {
                        left: parent.left
                        top: parent.top
                        right: parent.right
                    }
                    spacing: Kirigami.Units.largeSpacing * 2

                    ColumnLayout {
                        Kirigami.Icon {
                            Layout.alignment: Qt.AlignHCenter
                            source: location.currentWeather.weatherIcon
                            Layout.maximumHeight: Kirigami.Units.iconSizes.medium
                            Layout.preferredWidth: height
                            Layout.preferredHeight: Kirigami.Units.iconSizes.medium
                        }
                        Label {
                            Layout.alignment: Qt.AlignHCenter
                            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.4
                            text: location.currentWeather.temperature
                        }
                    }

                    Kirigami.Heading {
                        Layout.alignment: Qt.AlignLeft
                        level: 1
                        text: location.name
                    }

                    Item {
                        Layout.fillWidth: true
                    }
//                    Kirigami.ListItemDragHandle {
//                        Layout.alignment: Qt.AlignRight
//                        listItem: listItem
//                        listView: citiesList
//                        onMoveRequested: weatherLocationListModel.move(oldIndex, newIndex)
//                    }
                }
            }
        }
    }
}
