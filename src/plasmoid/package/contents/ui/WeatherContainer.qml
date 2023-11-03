/*
    SPDX-FileCopyrightText: 2021 HanY <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import org.kde.plasma.plasmoid
import org.kde.kirigami as Kirigami

Rectangle {
    id: container

    Layout.preferredWidth: Kirigami.Units.gridUnit * 12
    Layout.preferredHeight: Kirigami.Units.gridUnit * 12

    color: Kirigami.Theme.backgroundColor
    radius: 8

    MouseArea {
        anchors.fill: parent
        onDoubleClicked: window.showMaximized()
    }

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            Layout.fillWidth: true
            Label {
                text: Plasmoid.location
                color: Kirigami.Theme.textColor
                leftPadding: Kirigami.Units.smallSpacing
                font.bold: true
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
            }
            Label {
                id: temperatureLabel
                text: Plasmoid.temp + "°"
                color: Kirigami.Theme.activeTextColor
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Kirigami.Icon {
                source: Plasmoid.weatherIcon
            }

            Label {
                text: Plasmoid.desc
                color: Kirigami.Theme.textColor
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
            }
        }
        Row {
            Kirigami.Icon {
                source: "speedometer"
                isMask: true
                color: Kirigami.Theme.textColor
                Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                Layout.minimumWidth: Layout.minimumHeight * 1.5
            }
            Label {
                text: i18n("%1%", Plasmoid.humidity)
                color: Kirigami.Theme.textColor
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
            }
        }

        RowLayout {
            visible: Plasmoid.precipitation > 0.01
            Kirigami.Icon {
                source: "raindrop"
                isMask: true
                color: Kirigami.Theme.textColor
                Layout.preferredHeight: Kirigami.Units.iconSizes.medium
                Layout.preferredWidth: Kirigami.Units.iconSizes.medium
            }
            Label {
                text: i18n("%1mm", Plasmoid.precipitation.toFixed(1))
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
            }
        }
    }

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
            active: window.active
            anchors.fill: parent
            sourceComponent: Item {
                id: detailedItem
                anchors.fill: parent
                Rectangle {
                    width: Kirigami.Units.gridUnit * 21
                    height: Kirigami.Units.gridUnit * 22
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
                                    text: Plasmoid.location
                                }
                                Kirigami.Icon {
                                    source: Plasmoid.hourlyModel.currentIcon
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
                                    text: i18n("Select Location")
                                    onClicked: locationSelectDialog.open()
                                }
                                Button {
                                    Layout.alignment: Qt.AlignHCenter
                                    text: i18n("Open KWeather")
                                    onClicked: Plasmoid.hourlyModel.openKWeather()
                                }
                            }
                            // weather header
                            ColumnLayout {
                                id: headerText
                                Label {
                                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 3
                                    font.weight: Font.Light
                                    text: Plasmoid.hourlyModel.currentTemperature + "°"
                                }
                                Label {
                                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
                                    font.weight: Font.Bold
                                    text: Plasmoid.hourlyModel.currentDescription
                                }
                            }
                        }
                        ListView {
                            Layout.preferredHeight: Kirigami.Units.gridUnit * 16
                            Layout.fillWidth: true
                            model: Plasmoid.hourlyModel
                            delegate: Kirigami.BasicListItem {
                                label: time
                                subtitle: temperature + "°"
                                icon: weatherIcon
                            }
                        }
                    }
                }
                Dialog {
                    id: locationSelectDialog
                    standardButtons: Dialog.Close
                    title: i18n("Select Location")
                    anchors.centerIn: parent
                    width: Kirigami.Units.gridUnit * 12
                    height: Kirigami.Units.gridUnit * 12
                    LocationSelector {
                        anchors.fill: parent
                        onSelected: locationSelectDialog.close()
                    }
                }
            }
        }
    }
}
