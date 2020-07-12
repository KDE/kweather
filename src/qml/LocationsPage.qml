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
    property int currentIndex: 0;

    mainAction: Kirigami.Action {
        iconName: "list-add"
        text: i18n("Add Location")
        onTriggered: pageStack.push(addLocationPage)
    }

    Connections {
        target: weatherLocationListModel
        onNetworkErrorCreating: showPassiveNotification(i18n("Unable to fetch timezone information"))
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
            id: listItem
            actions: [
                Kirigami.Action {
                    iconName: "delete"
                    text: i18n("Remove")
                    onTriggered: {
                        weatherLocationListModel.remove(index);
                    }
                },
                Kirigami.Action {
                    iconName: "settings-configure"
                    text: i18n("Select Backend")
                    onTriggered: {
                        currentIndex = index;
                        selectBackend.open();
                    }
                }
            ]
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
                            source: location.currentWeather == null ? "weather-none-available" : location.currentWeather.weatherIcon
                            Layout.maximumHeight: Kirigami.Units.iconSizes.medium
                            Layout.preferredWidth: height
                            Layout.preferredHeight: Kirigami.Units.iconSizes.medium
                        }
                        Label {
                            Layout.alignment: Qt.AlignHCenter
                            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.4
                            text: location.currentWeather == null ? "0" : location.currentWeather.temperatureRounded
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
    // select backend dialog
    Dialog {
        property WeatherLocation mLocation: weatherLocationListModel.get(currentIndex)
        property var mBackend: mLocation.backend
        id: selectBackend
        modal: true
        focus: true
        x: (parent.width - width) / 2
        y: parent.height / 2 - height
        width: Math.min(parent.width - Kirigami.Units.gridUnit * 4, Kirigami.Units.gridUnit * 20)
        height: Kirigami.Units.gridUnit * 20
        title: i18n("Select Backend")
        standardButtons: Dialog.Close | Dialog.Save

        onAccepted: weatherLocationListModel.changeBackend(currentIndex,mBackend)
        onRejected: weatherLocationListModel.updateUi();

        contentItem: ScrollView {
            ListView {
                model: [i18nc("Norway Meteorologisk Institutt","Norway Meteorologisk Institutt"), i18nc("OpenWeatherMap","OpenWeatherMap")]
                delegate: RadioDelegate {
                    width: parent.width
                    text: modelData
                    checked: selectBackend.mBackend === modelData
                    enabled: !(modelData == "OpenWeatherMap") || (settingsModel.OWMToken.length != 0)
                    onCheckedChanged: {
                        if (checked) {
                            selectBackend.mBackend = modelData
                        }
                    }
                }
            }
            Component.onCompleted: background.visible = true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        }
    }
}
