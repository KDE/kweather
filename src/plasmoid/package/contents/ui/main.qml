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
    Plasmoid.fullRepresentation: Rectangle {
        id: container
        Layout.preferredWidth: Kirigami.Units.gridUnit * 12
        Layout.preferredHeight: Kirigami.Units.gridUnit * 12
        clip: true
        color: Kirigami.Theme.backgroundColor
        radius: 8

        SwipeView {
            id: forecastView
            anchors.fill: parent
            Repeater {
                model: plasmoid.nativeInterface.locationModel
                Loader {
                    visible: SwipeView.isCurrentItem
                    active: SwipeView.isCurrentItem || SwipeView.isNextItem || SwipeView.isPreviousItem
                    sourceComponent: Item {
                        MouseArea {
                            anchors.fill: parent
                            onDoubleClicked: {
                                window.showMaximized()
                            }
                        }

                        ColumnLayout {
                            anchors.fill: parent
                            RowLayout {
                                Layout.fillWidth: true
                                Label {
                                    text: locationName
                                    color: Kirigami.Theme.textColor
                                    leftPadding: Kirigami.Units.smallSpacing
                                    font.bold: true
                                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
                                }
                                Label {
                                    id: temperatureLabel
                                    text: temperature + "°"
                                    color: Kirigami.Theme.activeTextColor
                                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
                                }
                            }
                            Row {
                                spacing: Kirigami.Units.largeSpacing
                                Label {
                                    text: date
                                    color: Kirigami.Theme.disabledTextColor
                                    leftPadding: Kirigami.Units.smallSpacing
                                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
                                }
                                Kirigami.Icon {
                                    source: icon
                                }
                            }

                            Label {
                                text: description
                                color: Kirigami.Theme.textColor
                                leftPadding: Kirigami.Units.smallSpacing
                                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
                            }
                            Row {
                                Kirigami.Icon {
                                    source: "speedometer"
                                    Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                                    Layout.minimumWidth: Layout.minimumHeight * 1.5
                                }
                                Label {
                                    text: i18n("%1%", humidity)
                                    color: Kirigami.Theme.textColor
                                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
                                }
                            }

                            RowLayout {
                                visible: precipitation > 0.01
                                Kirigami.Icon {
                                    source: "raindrop"
                                    Layout.preferredHeight: Kirigami.Units.iconSizes.medium
                                    Layout.preferredWidth: Kirigami.Units.iconSizes.medium
                                }
                                Label {
                                    text: i18n("%1mm", precipitation.toFixed(1))
                                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
                                }
                            }
                        }
                    }
                }
            }
        }
        PageIndicator {
            visible: forecastView.count > 1
            currentIndex: forecastView.currentIndex
            count: forecastView.count
            anchors.right: parent.right
            anchors.bottom: parent.bottom
        }
    }
    Kirigami.AbstractApplicationWindow {
        id: window
        visible: false
        height: 100
        width: 500
        flags: Qt.FramelessWindowHint
        modality: Qt.WindowModal
        color: "transparent"
        MouseArea {
            anchors.fill: parent
            onClicked: window.close()
        }

        // TODO: content here
    }
}
