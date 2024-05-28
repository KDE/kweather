/*
 * SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
 * SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

Popup {
    id: setupPopup

    Kirigami.Theme.colorSet: Kirigami.Theme.Window

    modal: true
    focus: true
    width: parent.width - Kirigami.Units.largeSpacing * 4
    height: parent.height - Kirigami.Units.largeSpacing * 4
    parent: Overlay.overlay
    x: Math.round((parent.width - width) / 2)
    y: Math.round((parent.height - height) / 2)
    closePolicy: Popup.NoAutoClose
    padding: Kirigami.Units.largeSpacing

    background: Kirigami.ShadowedRectangle {
        color: Kirigami.Theme.backgroundColor
        radius: Kirigami.Units.largeSpacing
        anchors.fill: parent

        shadow.size: Kirigami.Units.largeSpacing
        shadow.color: Qt.rgba(0.0, 0.0, 0.0, 0.55)
        shadow.yOffset: 2
    }

    SwipeView {
        id: view
        anchors.fill: parent
        clip: true
        currentIndex: 0
        interactive: false

        // landing page
        Item {
            ColumnLayout {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: Kirigami.Units.largeSpacing * 3

                Kirigami.Icon {
                    source: "qrc:/resources/kweather.svg"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    implicitHeight: Kirigami.Units.gridUnit * 8
                    implicitWidth: implicitHeight
                }

                Label {
                    font.pointSize: Math.round(Kirigami.Theme.defaultFont.pointSize * 1.5)
                    font.weight: Font.Normal
                    text: i18n("Welcome to Weather")
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    wrapMode: Label.WordWrap
                }

                ToolButton {
                    text: i18n("Continue")
                    icon.name: "go-next-symbolic"
                    onClicked: view.currentIndex++
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    flat: false
                }
            }
        }

        // select forecast style page
        Item {
            ColumnLayout {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: Kirigami.Units.largeSpacing * 2

                Label {
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
                    text: i18n("Select forecast theme")
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                }

                Label {
                    text: i18n("You can change the theme later in the settings.")
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                }

                RowLayout {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    spacing: Kirigami.Units.largeSpacing

                    // flat
                    Kirigami.ShadowedRectangle {
                        color: settingsModel.forecastStyle === "Flat" ? Kirigami.ColorUtils.tintWithAlpha(Kirigami.Theme.backgroundColor, Kirigami.Theme.highlightColor, 0.3) : Kirigami.Theme.backgroundColor
                        radius: Kirigami.Units.smallSpacing
                        width: flatColumn.width + Kirigami.Units.largeSpacing * 2
                        height: flatColumn.height + Kirigami.Units.largeSpacing * 2

                        Behavior on color {
                            ColorAnimation {
                                duration: Kirigami.Units.longDuration
                                easing.type: Easing.InOutQuad
                            }
                        }

                        border.width: 1
                        border.color: Qt.tint(Kirigami.Theme.textColor, Qt.rgba(color.r, color.g, color.b, 0.6))

                        shadow.size: Kirigami.Units.largeSpacing
                        shadow.color: Qt.rgba(0.0, 0.0, 0.0, 0.15)
                        shadow.yOffset: Kirigami.Units.devicePixelRatio * 2

                        ColumnLayout {
                            id: flatColumn
                            anchors.centerIn: parent
                            spacing: -Kirigami.Units.gridUnit * 2
                            width: Kirigami.Units.gridUnit * 7
                            height: Kirigami.Units.gridUnit * 14
                            Label {
                                text: i18n("Flat")
                                Layout.alignment: Qt.AlignHCenter
                            }

                            Image {
                                asynchronous: true
                                Layout.maximumWidth: parent.width
                                fillMode: Image.PreserveAspectFit
                                source: "qrc:/resources/KWeather_FLAT.png"
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: settingsModel.forecastStyle = "Flat"
                        }
                    }

                    // dynamic
                    Kirigami.ShadowedRectangle {
                        color: settingsModel.forecastStyle === "Dynamic" ? Kirigami.ColorUtils.tintWithAlpha(Kirigami.Theme.backgroundColor, Kirigami.Theme.highlightColor, 0.3) : Kirigami.Theme.backgroundColor
                        radius: Kirigami.Units.smallSpacing
                        width: dynamicColumn.width + Kirigami.Units.largeSpacing * 2
                        height: dynamicColumn.height + Kirigami.Units.largeSpacing * 2

                        Behavior on color {
                            ColorAnimation {
                                duration: Kirigami.Units.longDuration
                                easing.type: Easing.InOutQuad
                            }
                        }

                        border.width: 1
                        border.color: Qt.tint(Kirigami.Theme.textColor, Qt.rgba(color.r, color.g, color.b, 0.6))

                        shadow.size: Kirigami.Units.largeSpacing
                        shadow.color: Qt.rgba(0.0, 0.0, 0.0, 0.15)
                        shadow.yOffset: 2

                        ColumnLayout {
                            id: dynamicColumn
                            anchors.centerIn: parent
                            spacing: -Kirigami.Units.gridUnit * 2
                            width: Kirigami.Units.gridUnit * 7
                            height: Kirigami.Units.gridUnit * 14
                            Label {
                                text: i18n("Dynamic")
                                Layout.alignment: Qt.AlignHCenter
                            }

                            Image {
                                asynchronous: true
                                Layout.maximumWidth: parent.width
                                fillMode: Image.PreserveAspectFit
                                source: "qrc:/resources/KWeather_DYNAMIC.png"
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: settingsModel.forecastStyle = "Dynamic"
                        }
                    }
                }

                RowLayout {
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    spacing: Kirigami.Units.largeSpacing
                    ToolButton {
                        text: i18n("Back")
                        icon.name: "go-previous-symbolic"
                        flat: false
                        onClicked: view.currentIndex--
                    }
                    ToolButton {
                        text: i18n("Finish")
                        icon.name: "go-next-symbolic"
                        flat: false
                        onClicked: {
                            // setup finished
                            setupPopup.close();
                            settingsModel.firstStartup = false;
                            settingsModel.save();
                        }
                    }
                }
            }
        }
    }
}
