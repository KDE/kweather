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
        id: container
        Layout.preferredWidth: 100
        Layout.preferredHeight: 100
        SwipeView {
            id: forecastView
            anchors.fill: parent
            Repeater {
                model: plasmoid.nativeInterface.locationModel
                Loader {
                    visible: SwipeView.isCurrentItem
                    active: SwipeView.isCurrentItem || SwipeView.isNextItem || SwipeView.isPreviousItem
                    sourceComponent: Kirigami.Icon {
                        id: mainComponent
                        source: icon
                        ColumnLayout {
                            RowLayout {
                                Layout.preferredWidth: parent.width
                                Label {
                                    width: parent.width - temperatureLabel.width
                                    text: locationName
                                    color: Kirigami.Theme.textColor
                                    leftPadding: Kirigami.Units.smallSpacing
                                    font.bold: true
                                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 2
                                }
                                Label {
                                    id: temperatureLabel
                                    text: temperature + "°"
                                    color: Kirigami.Theme.disabledTextColor
                                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
                                }
                            }
                            Label {
                                text: description
                                color: Kirigami.Theme.textColor
                                leftPadding: Kirigami.Units.smallSpacing
                                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
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
            currentIndex: forecastView.currentIndex
            count: forecastView.count
            anchors.right: parent.right
            anchors.bottom: parent.bottom
        }
    }
}
