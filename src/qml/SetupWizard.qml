/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.12 as Kirigami
import kweather 1.0

Kirigami.ApplicationWindow
{
    id: appwindow
    title: i18n("Weather")

    width: Kirigami.Units.gridUnit * 27
    height: Kirigami.Units.gridUnit * 45
   
    SwipeView {
        id: view
        anchors.fill: parent
        currentIndex: 0
        interactive: false
        
        // landing page
        Item {
            Rectangle {
                id: landingPage
                color: Kirigami.Theme.viewBackgroundColor
                width: parent.width
                height: parent.height
                
                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: Kirigami.Units.largeSpacing * 3
                    
                    Label {
                        font.pointSize: Kirigami.Theme.defaultFont.pointSize * 2
                        font.weight: Font.Light
                        text: i18n("Welcome to KWeather")
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    }
                    
                    Kirigami.Icon {
                        source: "qrc:/resources/kweather.svg"
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        implicitHeight: Kirigami.Units.gridUnit * 8
                        implicitWidth: implicitHeight
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
        }
        
        // select forecast style page
        Item {
            Rectangle {
                id: themePage
                color: Kirigami.Theme.viewBackgroundColor
                width: parent.width
                height: parent.height
                
                ColumnLayout {
                    anchors.centerIn: parent
                    spacing: Kirigami.Units.largeSpacing * 3
                    
                    Label {
                        font.pointSize: Kirigami.Theme.defaultFont.pointSize * 2
                        font.weight: Font.Light
                        text: i18n("Select forecast theme")
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    }
                    
                    Label {
                        text: i18n("You can change the theme later in the settings.")
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    }
                    
                    Flow {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        spacing: Kirigami.Units.largeSpacing
                        
                        // flat
                        Kirigami.ShadowedRectangle {
                            color: settingsModel.forecastStyle === "Flat" ? Kirigami.Theme.highlightColor : Kirigami.Theme.viewBackgroundColor
                            radius: Kirigami.Units.smallSpacing
                            width: flatColumn.width + Kirigami.Units.largeSpacing * 2
                            height: flatColumn.height + Kirigami.Units.largeSpacing * 2

                            shadow.size: Kirigami.Units.largeSpacing
                            shadow.color: Qt.rgba(0.0, 0.0, 0.0, 0.15)
                            shadow.yOffset: Kirigami.Units.devicePixelRatio * 2
                            
                            ColumnLayout {
                                id: flatColumn
                                spacing: Kirigami.Units.largeSpacing
                                anchors.centerIn: parent
                                Label {
                                    text: i18n("Flat (Performance)")
                                    Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                                }
                                
                                Image {
                                    width: Kirigami.Units.gridUnit * 14
                                    fillMode: Image.PreserveAspectFit
                                    source: "qrc:/resources/KWeather_FLAT.png"
                                    Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                                }
                            }
                            
                            MouseArea {
                                anchors.fill: parent
                                onClicked: settingsModel.forecastStyle = "Flat"
                            }
                        }
                        
                        // dynamic
                        Kirigami.ShadowedRectangle {
                            color: settingsModel.forecastStyle === "Dynamic" ? Kirigami.Theme.highlightColor : Kirigami.Theme.viewBackgroundColor
                            radius: Kirigami.Units.smallSpacing
                            width: dynamicColumn.width + Kirigami.Units.largeSpacing * 2
                            height: dynamicColumn.height + Kirigami.Units.largeSpacing * 2

                            shadow.size: Kirigami.Units.largeSpacing
                            shadow.color: Qt.rgba(0.0, 0.0, 0.0, 0.15)
                            shadow.yOffset: Kirigami.Units.devicePixelRatio * 2
                            
                            ColumnLayout {
                                id: dynamicColumn
                                spacing: Kirigami.Units.largeSpacing
                                anchors.centerIn: parent
                                Label {
                                    text: i18n("Dynamic (Animated)")
                                    Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                                }
                                
                                Image {
                                    width: Kirigami.Units.gridUnit * 14
                                    fillMode: Image.PreserveAspectFit
                                    source: "qrc:/resources/KWeather_DYNAMIC.png"
                                    Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                                }
                            }
                            
                            MouseArea {
                                anchors.fill: parent
                                onClicked: settingsModel.forecastStyle = "Dynamic"
                            }
                        }
                    }
                    
                    ToolButton {
                        text: i18n("Finish")
                        icon.name: "go-next-symbolic"
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                        flat: false
                        onClicked: {
                            // setup finished
                            //mainLoader.source = "main.qml";
                            appwindow.visible = false;
                            appwindow.close();
                            settingsModel.firstStartup = false;
                        }
                    }
                }
            }
        }
    }
}
