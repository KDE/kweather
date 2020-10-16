/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.11 as Kirigami
import kweather 1.0

Kirigami.ApplicationWindow
{
    id: appwindow
    title: i18n("Weather")

    width: Kirigami.Units.gridUnit * 27
    height: Kirigami.Units.gridUnit * 45
    Rectangle {
        id: headerText
        color: "#3daee2"
        width: parent.width
        height: headerLayout.height
        ColumnLayout {
            id: headerLayout
            Layout.fillWidth: true
            Label {
                text: i18n("Select theme")
                Layout.fillWidth: true
                color: "white"
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 5
            }

            Label {
                text: i18n("You can change the theme later too")
                Layout.fillWidth: true
                color: "white"
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 2
            }
        }
    }


    Rectangle {
        id: themeImgBackground
        color: "#aaaaaa"
        anchors.top: headerText.bottom
        width: parent.width
        height: themeImg.height
        Image {
            id: themeImg
            width: parent.width
            fillMode: Image.PreserveAspectFit
            source: "qrc:/resources/KWeather_DYNAMIC.png"
        }
    }

    ListView {
        id: themeView
        anchors.top: themeImgBackground.bottom
        width: parent.width
        height: 500
        model: ListModel {
            ListElement {
                name: "Dynamic"
            }
            ListElement {
                name: "Flat"
            }
        }

        delegate: Kirigami.BasicListItem {
            id: wrapper
            Label {
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 2
                text: name
                color: wrapper.ListView.isCurrentItem ? "white" : "#3daee2"
                Layout.alignment: Qt.AlignLeft
            }
        }

        onCurrentIndexChanged: {
            if(currentIndex == 1){
                themeImg.source = "qrc:/resources/KWeather_FLAT.png"
                settingsModel.forecastStyle = "Flat"
            }
            else{
                themeImg.source = "qrc:/resources/KWeather_DYNAMIC.png"
                settingsModel.forecastStyle = "Dynamic"
            }
        }
    }

    Loader {
        id: mainLoader
    }

    footer: Item {
        height: Kirigami.Units.gridUnit + confirmBtn.height
        Button {
            id: confirmBtn
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            text: i18n("NEXT")
            onClicked: {
                mainLoader.source = "main.qml";
                appwindow.visible = false;
            }
        }
    }
}
