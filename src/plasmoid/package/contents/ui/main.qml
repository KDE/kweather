/*
    SPDX-FileCopyrightText: 2020 HanY <hanyoung@protonmail.com>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

import QtQuick 2.1
import QtQuick.Layouts 1.1
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.kirigami 2.11 as Kirigami

Item {
    Plasmoid.backgroundHints: "NoBackground";
    Plasmoid.fullRepresentation: Item {
        RowLayout {
            id: weatherHeader
            ColumnLayout {
                Text {
                    text: plasmoid.nativeInterface.cityName
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
                }
                Text {
                    text: plasmoid.nativeInterface.tempNow
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 2
                }
                Text {
                    text: plasmoid.nativeInterface.description
                }
            }

            ColumnLayout {
                id: weatherList
                Flow {
                    Repeater {
                        model: plasmoid.nativeInterface.date
                        Text {
                            text: modelData
                            width: 80
                            horizontalAlignment: Qt.AlignHCenter
                        }
                    }
                }
                Flow {
                    Repeater {
                        model: plasmoid.nativeInterface.forecast
                        Kirigami.Icon {
                            source: modelData
                            width: 80
                            height: 50
                        }
                    }
                }
                Flow {
                    Repeater {
                        model: plasmoid.nativeInterface.maxMinTemp
                        Text {
                            text: modelData
                            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 0.8
                            width: 80
                            horizontalAlignment: Qt.AlignHCenter
                        }
                    }
                }
            }
        }
    }
}
