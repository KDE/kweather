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
    Plasmoid.fullRepresentation: Item {
        Layout.preferredWidth: 200 //forecastView.width
        Layout.preferredHeight: 150
        Text {
            text: "debug"
        }

        SwipeView {
            id: forecastView
            anchors.fill: parent
            Repeater {
                model: plasmoid.nativeInterface.locationModel
                Loader {
                    active: SwipeView.isCurrentItem || SwipeView.isNextItem || SwipeView.isPreviousItem
                    sourceComponent: RowLayout {
                        Label {
                            text: locationName
                        }
                        Label {
                            text: temperature
                        }
                        Kirigami.Icon {
                            source: icon
                        }
                        Label {
                            text: description
                        }
                    }
                }
            }
        }

//        RowLayout {
//            id: weatherHeader
//            ColumnLayout {
//                Text {
//                    text: plasmoid.nativeInterface.cityName
//                    color: "white"
//                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
//                }
//                Text {
//                    text: plasmoid.nativeInterface.tempNow
//                    color: "white"
//                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 2
//                }
//                Text {
//                    text: plasmoid.nativeInterface.description
//                    color: "white"
//                }
//            }

//            ColumnLayout {
//                id: weatherList
//                Flow {
//                    Repeater {
//                        model: plasmoid.nativeInterface.date
//                        Text {
//                            text: modelData
//                            color: "white"
//                            width: Kirigami.Units.gridUnit * 4
//                            horizontalAlignment: Qt.AlignHCenter
//                        }
//                    }
//                }
//                Flow {
//                    Repeater {
//                        model: plasmoid.nativeInterface.forecast
//                        Kirigami.Icon {
//                            source: modelData
//                            width: Kirigami.Units.gridUnit * 4
//                            height: Kirigami.Units.gridUnit * 3
//                        }
//                    }
//                }
//                Flow {
//                    Repeater {
//                        model: plasmoid.nativeInterface.maxMinTemp
//                        Text {
//                            text: modelData
//                            color: "white"
//                            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 0.8
//                            width: Kirigami.Units.gridUnit * 4
//                            horizontalAlignment: Qt.AlignHCenter
//                        }
//                    }
//                }
//            }

//            ToolButton {
//                Layout.fillHeight: true
//                Layout.preferredWidth: Kirigami.Units.gridUnit * 1.5
//                visible: !plasmoid.nativeInterface.isSingleLocation
//                flat: false
//                icon.name: "arrow-right"
//                onClicked: plasmoid.nativeInterface.nextLocation()
//            }
//        }
    }
}
