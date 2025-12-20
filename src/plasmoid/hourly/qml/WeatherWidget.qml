/*
    SPDX-FileCopyrightText: 2021 HanY <hanyoung@protonmail.com>
    SPDX-FileCopyrightText: 2025 Devin Lin <devin@kde.org>
    SPDX-License-Identifier: LGPL-2.1-or-later
*/

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2

import org.kde.plasma.plasmoid
import org.kde.kirigami as Kirigami

Item {
    id: root

    Kirigami.Theme.inherit: false
    Kirigami.Theme.colorSet: Kirigami.Theme.Complementary

    property real margins: (root.height > Kirigami.Units.gridUnit * 12) ? Kirigami.Units.gridUnit : Kirigami.Units.largeSpacing

    RowLayout {
        anchors.topMargin: root.margins
        anchors.leftMargin: Kirigami.Units.gridUnit
        anchors.rightMargin: Kirigami.Units.gridUnit
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right

        spacing: Kirigami.Units.largeSpacing

        ColumnLayout {
            spacing: Kirigami.Units.smallSpacing
            Layout.fillWidth: true
            Layout.fillHeight: true

            QQC2.Label {
                Layout.fillWidth: true
                text: Plasmoid.temp + "°"
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 2
                font.weight: Font.Light
            }
            QQC2.Label {
                Layout.fillWidth: true
                elide: Text.ElideRight
                text: Plasmoid.locationName
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 0.8
            }
        }

        ColumnLayout {
            spacing: Kirigami.Units.smallSpacing
            Layout.fillHeight: true

            Kirigami.Icon {
                Layout.alignment: Qt.AlignRight | Qt.AlignTop
                implicitWidth: Kirigami.Units.iconSizes.medium
                implicitHeight: Kirigami.Units.iconSizes.medium
                source: Plasmoid.weatherIcon + '-symbolic'
            }
            QQC2.Label {
                text: Plasmoid.desc
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 0.8
            }
        }
    }

    Rectangle {
        id: bottomScrim
        color: Qt.rgba(0, 0, 0, 0.3)
        radius: Kirigami.Units.gridUnit

        height: rowLayout.height + root.margins * 2
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    RowLayout {
        id: rowLayout

        anchors.margins: root.margins
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Repeater {
            model: Plasmoid.hourlyModel

            delegate: ColumnLayout {
                property int rows: Math.min(Math.floor(root.width / (Kirigami.Units.gridUnit * 4)), 6)

                width: rowLayout.width / rows
                visible: model.index < rows

                QQC2.Label {
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignHCenter
                    text: model.time
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 0.7
                }
                Kirigami.Icon {
                    Layout.alignment: Qt.AlignHCenter
                    implicitWidth: Kirigami.Units.iconSizes.smallMedium
                    implicitHeight: Kirigami.Units.iconSizes.smallMedium
                    source: model.weatherIcon + '-symbolic'
                }
                QQC2.Label {
                    Layout.fillWidth: true
                    horizontalAlignment: Text.AlignHCenter
                    text: model.temperature + "°"
                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 0.7
                }
            }
        }
    }
}
