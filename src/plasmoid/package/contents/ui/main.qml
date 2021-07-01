/*
    SPDX-FileCopyrightText: 2020 HanY <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

import QtQuick 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.4
import org.kde.plasma.plasmoid 2.0
import org.kde.kirigami 2.11 as Kirigami
Item {
    Plasmoid.backgroundHints: "ShadowBackground";
    Plasmoid.fullRepresentation: Loader {
        active: true
        source: plasmoid.nativeInterface.needLocation ? "LocationSelector.qml" : "WeatherContainer.qml"
    }

    /*
    Kirigami.AbstractApplicationWindow {
        id: window
        visible: false
        flags: Qt.FramelessWindowHint
        modality: Qt.WindowModal
        color: "transparent"
        MouseArea {
            anchors.fill: parent
            onClicked: window.close()
        }

        // TODO: content here
        Loader {
            active: window.visible
            anchors.fill: parent
            sourceComponent: Item {
                id: detailedItem
                anchors.fill: parent
                Rectangle {
                    width: Kirigami.Units.gridUnit * 20
                    height: Kirigami.Units.gridUnit * 21
                    radius: Kirigami.Units.gridUnit
                    anchors.centerIn: parent
                    RowLayout {
                        anchors.fill: parent
                        RowLayout {
                            Layout.alignment: Qt.AlignHCenter
                            ColumnLayout {
                                Label {
                                    Layout.alignment: Qt.AlignHCenter
                                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 2
                                    font.weight: Font.Light
                                    text: plasmoid.nativeInterface.hourlyModel.location
                                }
                                Kirigami.Icon {
                                    source: plasmoid.nativeInterface.hourlyModel.currentIcon
                                    Layout.preferredHeight: width
                                    Layout.preferredWidth: detailedItem.width * 0.8 - headerText.width
                                    Layout.maximumHeight: Kirigami.Theme.defaultFont.pointSize * 15
                                    Layout.maximumWidth: Kirigami.Theme.defaultFont.pointSize * 15
                                    Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 5
                                    Layout.minimumWidth: Kirigami.Theme.defaultFont.pointSize * 5
                                    smooth: true
                                }
                                Button {
                                    Layout.alignment: Qt.AlignHCenter
                                    text: i18n("Open KWeather")
                                    onClicked: plasmoid.nativeInterface.hourlyModel.openKWeather()
                                }
                            }
                            // weather header
                            ColumnLayout {
                                id: headerText
                                Label {
                                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 3
                                    font.weight: Font.Light
                                    text: plasmoid.nativeInterface.hourlyModel.currentTemperature + "°"
                                }
                                Label {
                                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
                                    font.weight: Font.Bold
                                    text: plasmoid.nativeInterface.hourlyModel.currentDescription
                                }
                            }
                        }
                        ListView {
                            Layout.preferredHeight: Kirigami.Units.gridUnit * 16
                            Layout.preferredWidth: Kirigami.Units.gridUnit * 5
                            model: plasmoid.nativeInterface.hourlyModel
                            delegate: Kirigami.BasicListItem {
                                label: time
                                subtitle: temperature + "°"
                                icon: weatherIcon
                                //backgroundColor: Kirigami.Theme.backgroundColor
                            }
                        }
                    }
                }
            }
        }
    }
    */
}
