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
import org.kde.kirigamiaddons.formcard 1.0 as FormCard

import org.kde.kweather.backgrounds
import org.kde.kweather.backgrounds.components
import org.kde.kweather.settings
import org.kde.kweather.locations

Popup {
    id: root

    Kirigami.Theme.colorSet: Kirigami.Theme.Window

    modal: true
    focus: true
    width: parent.width
    height: parent.height
    parent: Overlay.overlay
    closePolicy: Popup.NoAutoClose
    padding: Kirigami.Units.largeSpacing

    topPadding: 0
    bottomPadding: 0
    leftPadding: 0
    rightPadding: 0

    property bool viewContainerOpen: false

    // States:
    // !viewContainerOpen: Weather view
    // Page 0: Appearance
    // Page 1: Units
    // Page 2: Add first location

    readonly property string appearanceHeading: i18n('Appearance')
    readonly property string unitsHeading: i18n('Units')
    readonly property string addLocationHeading: i18n('Add Location')

    function requestNextPage() {
        if (!viewContainerOpen) {
            viewContainerOpen = true;
            stepHeading.changeText(appearanceHeading);
            return;
        }

        switch (swipeView.currentIndex) {
            case 0:
                stepHeading.changeText(unitsHeading);
                swipeView.currentIndex++;
                break;
            case 1:
                stepHeading.changeText(addLocationHeading)
                swipeView.currentIndex++;
                break;
            case 2:
                finishSetup();
                break;
        }
    }

    function requestPreviousPage() {
        switch (swipeView.currentIndex) {
            case 0:
                root.viewContainerOpen = false;
                stepHeading.changeText('');
                break;
            case 1:
                stepHeading.changeText(appearanceHeading);
                swipeView.currentIndex--;
                break;
            case 2:
                stepHeading.changeText(unitsHeading);
                swipeView.currentIndex--;
                break;
        }
    }

    function finishSetup() {
        // setup finished
        root.close();
        settingsModel.firstStartup = false;
        settingsModel.save();
    }

    readonly property real cardWidth: Math.min(Kirigami.Units.gridUnit * 30, swipeView.width - Kirigami.Units.gridUnit * 2)

    // animated background
    background: Rectangle {
        id: rect

        // Switch backgrounds every 3 seconds
        Loader {
            id: dynamicLoader
            sourceComponent: cloudyDay

            Component { id: cloudyDay; CloudyDay {} }
            Component { id: rainyDay; RainyDay {} }
            Component { id: rainyNight; RainyNight {} }
            Component { id: clearNight; ClearNight {} }
            Component { id: clearDay; ClearDay {} }
            Component { id: snowyDay; SnowyDay {} }

            Timer {
                running: true
                repeat: true
                interval: 3000
                onTriggered: {
                    if (dynamicLoader.sourceComponent === cloudyDay) {
                        dynamicLoader.sourceComponent = rainyDay;
                    } else if (dynamicLoader.sourceComponent === rainyDay) {
                        dynamicLoader.sourceComponent = rainyNight;
                    } else if (dynamicLoader.sourceComponent === rainyNight) {
                        dynamicLoader.sourceComponent = clearNight;
                    } else if (dynamicLoader.sourceComponent === clearNight) {
                        dynamicLoader.sourceComponent = clearDay;
                    } else if (dynamicLoader.sourceComponent === clearDay) {
                        dynamicLoader.sourceComponent = snowyDay;
                    } else if (dynamicLoader.sourceComponent === snowyDay) {
                        dynamicLoader.sourceComponent = cloudyDay;
                    }
                }
            }
        }
        property DynamicBackground dynamicBackground: dynamicLoader.item

        // background colours
        gradient: Gradient {
            GradientStop {
                color: rect.dynamicBackground.gradientColorTop
                position: 0.0
                Behavior on color {
                    ColorAnimation {
                        duration: Kirigami.Units.longDuration
                    }
                }
            }
            GradientStop {
                color: rect.dynamicBackground.gradientColorBottom
                position: 1.0
                Behavior on color {
                    ColorAnimation {
                        duration: Kirigami.Units.longDuration
                    }
                }
            }
        }

        Loader {
            anchors.fill: parent
            opacity: rect.dynamicBackground.sun ? 1 : 0
            Behavior on opacity { NumberAnimation { duration: Kirigami.Units.longDuration } }
            active: opacity !== 0
            sourceComponent: Sun {}
        }
        Loader {
            anchors.fill: parent
            opacity: rect.dynamicBackground.stars ? 1 : 0
            Behavior on opacity { NumberAnimation { duration: Kirigami.Units.longDuration } }
            active: opacity !== 0
            sourceComponent: Stars {}
        }
        Loader {
            anchors.fill: parent
            opacity: rect.dynamicBackground.clouds ? 1 : 0
            Behavior on opacity { NumberAnimation { duration: Kirigami.Units.longDuration } }
            active: opacity !== 0
            sourceComponent: Cloudy {}
        }
        Loader {
            anchors.fill: parent
            opacity: rect.dynamicBackground.rain ? 1 : 0
            Behavior on opacity { NumberAnimation { duration: Kirigami.Units.longDuration } }
            active: opacity !== 0
            sourceComponent: Rain {}
        }
        Loader {
            anchors.fill: parent
            opacity: rect.dynamicBackground.snow ? 1 : 0
            Behavior on opacity { NumberAnimation { duration: Kirigami.Units.longDuration } }
            active: opacity !== 0
            sourceComponent: Snow {}
        }

        // scrim for when the drawer is open
        Rectangle {
            anchors.fill: parent
            color: 'black'
            opacity: root.viewContainerOpen ? 0.5 : 0.4
            Behavior on opacity { NumberAnimation { duration: Kirigami.Units.longDuration } }
        }
    }

    contentItem: Item {
        width: root.width
        height: root.height

        ColumnLayout {
            id: introBlock
            spacing: Kirigami.Units.gridUnit
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            opacity: viewContainerOpen ? 0 : 1
            Behavior on opacity {
                NumberAnimation {
                    duration: 1000
                    easing.type: Easing.OutExpo
                }
            }

            Kirigami.Theme.inherit: false
            Kirigami.Theme.colorSet: Kirigami.Theme.Complementary

            Kirigami.Icon {
                source: "qrc:/resources/kweather.svg"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                implicitHeight: Kirigami.Units.gridUnit * 4
                implicitWidth: implicitHeight
            }

            Label {
                Layout.fillWidth: true
                Layout.leftMargin: Kirigami.Units.gridUnit * 2
                Layout.rightMargin: Kirigami.Units.gridUnit * 2
                Layout.bottomMargin: Kirigami.Units.gridUnit
                font.pointSize: 20
                text: i18n('Welcome to <b>Weather</b>')
                horizontalAlignment: Text.AlignHCenter
            }

            // Info rows
            RowLayout {
                spacing: Kirigami.Units.largeSpacing
                Layout.maximumWidth: Kirigami.Units.gridUnit * 20
                Layout.fillWidth: true
                Layout.leftMargin: Kirigami.Units.gridUnit * 2
                Layout.rightMargin: Kirigami.Units.gridUnit * 2
                Layout.alignment: Qt.AlignHCenter

                Kirigami.Icon {
                    source: 'weather-freezing-scattered-rain-storm-night-symbolic'
                    implicitWidth: Kirigami.Units.iconSizes.medium
                    implicitHeight: Kirigami.Units.iconSizes.medium
                }
                Label {
                    Layout.fillWidth: true
                    wrapMode: Text.Wrap
                    opacity: 0.8
                    font: Kirigami.Theme.smallFont
                    text: i18n('View hourly and daily weather forecasts, along with other detailed information such as wind speed and precipitation.')
                }
            }
            RowLayout {
                spacing: Kirigami.Units.largeSpacing
                Layout.maximumWidth: Kirigami.Units.gridUnit * 20
                Layout.fillWidth: true
                Layout.leftMargin: Kirigami.Units.gridUnit * 2
                Layout.rightMargin: Kirigami.Units.gridUnit * 2
                Layout.alignment: Qt.AlignHCenter

                Kirigami.Icon {
                    source: 'weather-clear-night-symbolic'
                    implicitWidth: Kirigami.Units.iconSizes.medium
                    implicitHeight: Kirigami.Units.iconSizes.medium
                }
                Label {
                    Layout.fillWidth: true
                    wrapMode: Text.Wrap
                    opacity: 0.8
                    font: Kirigami.Theme.smallFont
                    text: i18n('Save multiple locations and switch between them easily with the locations list.')
                }
            }

            RowLayout {
                spacing: Kirigami.Units.largeSpacing
                Layout.maximumWidth: Kirigami.Units.gridUnit * 20
                Layout.fillWidth: true
                Layout.leftMargin: Kirigami.Units.gridUnit * 2
                Layout.rightMargin: Kirigami.Units.gridUnit * 2
                Layout.alignment: Qt.AlignHCenter

                Kirigami.Icon {
                    source: 'weather-clear-symbolic'
                    implicitWidth: Kirigami.Units.iconSizes.medium
                    implicitHeight: Kirigami.Units.iconSizes.medium
                }
                Label {
                    Layout.fillWidth: true
                    wrapMode: Text.Wrap
                    opacity: 0.8
                    font: Kirigami.Theme.smallFont
                    text: i18n('Built by the community, and powered by data from the Norwegian Meteorological Institute.')
                }
            }

            Button {
                Layout.topMargin: Kirigami.Units.gridUnit
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                text: i18n("Get Started")
                icon.name: "go-next-symbolic"
                onClicked: root.requestNextPage();
            }
        }

        Item {
            id: viewContainer
            anchors.fill: parent

            property real translateY: viewContainerOpen ? 0 : viewContainer.height
            opacity: viewContainerOpen ? 1 : 0
            transform: Translate { y: viewContainer.translateY }

            Behavior on opacity {
                NumberAnimation {
                    duration: 1000
                    easing.type: Easing.OutExpo
                }
            }

            Behavior on translateY {
                NumberAnimation {
                    duration: 1000
                    easing.type: Easing.OutExpo
                }
            }

            // Heading for wizard steps
            Label {
                id: stepHeading
                opacity: 0
                color: "white"
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 18
                font.bold: true

                anchors.left: parent.left
                anchors.leftMargin: Kirigami.Units.gridUnit
                anchors.right: parent.right
                anchors.rightMargin: Kirigami.Units.gridUnit
                anchors.bottom: parent.bottom
                anchors.bottomMargin: root.height * 0.7 + Kirigami.Units.gridUnit

                property string toText

                function changeText(text) {
                    toText = text;
                    toHidden.restart();
                }

                NumberAnimation on opacity {
                    id: toHidden
                    duration: 200
                    to: 0
                    onFinished: {
                        stepHeading.text = stepHeading.toText;
                        toShown.restart();
                    }
                }

                NumberAnimation on opacity {
                    id: toShown
                    duration: 200
                    to: 1
                }
            }

            // Wizard content container
            Item {
                id: swipeViewContainer

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                height: Math.round(parent.height * 0.7)
                width: Math.min(parent.width, Kirigami.Units.gridUnit * 28)

                Rectangle {
                    radius: Kirigami.Units.gridUnit
                    color: Kirigami.Theme.backgroundColor
                    anchors.fill: parent
                    anchors.bottomMargin: -Kirigami.Units.gridUnit * 2
                }

                // bottom footer
                RowLayout {
                    id: swipeViewFooter
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Button {
                        Layout.alignment: Qt.AlignLeft
                        Layout.leftMargin: Kirigami.Units.gridUnit
                        Layout.bottomMargin: Kirigami.Units.gridUnit

                        topPadding: Kirigami.Units.largeSpacing
                        bottomPadding: Kirigami.Units.largeSpacing
                        leftPadding: Kirigami.Units.gridUnit
                        rightPadding: Kirigami.Units.gridUnit

                        text: i18n("Back")
                        icon.name: "arrow-left"

                        onClicked: root.requestPreviousPage()
                    }

                    Item {}

                    Button {
                        Layout.alignment: Qt.AlignRight
                        Layout.rightMargin: Kirigami.Units.gridUnit
                        Layout.bottomMargin: Kirigami.Units.gridUnit

                        topPadding: Kirigami.Units.largeSpacing
                        bottomPadding: Kirigami.Units.largeSpacing
                        leftPadding: Kirigami.Units.gridUnit
                        rightPadding: Kirigami.Units.gridUnit

                        visible: !root.onFinalPage
                        text: i18n("Next")
                        icon.name: "arrow-right"

                        onClicked: root.requestNextPage();
                    }
                }

                SwipeView {
                    id: swipeView
                    anchors.fill: parent
                    clip: true
                    currentIndex: 0
                    interactive: false

                    // Appearance page
                    Item {
                        ColumnLayout {
                            spacing: Kirigami.Units.gridUnit
                            anchors.fill: parent
                            anchors.topMargin: Kirigami.Units.gridUnit
                            anchors.bottomMargin: Kirigami.Units.gridUnit + swipeViewFooter.height

                            Label {
                                Layout.fillWidth: true
                                text: i18n("The style can be changed later in the settings.")
                                horizontalAlignment: Text.AlignHCenter
                            }

                            FormCard.FormCard {
                                maximumWidth: root.cardWidth
                                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                                Layout.fillWidth: true

                                FormCard.AbstractFormDelegate {
                                    id: dynamicStyleDelegate
                                    onClicked: settingsModel.forecastStyle = "Dynamic"

                                    contentItem: RowLayout {
                                        spacing: Kirigami.Units.largeSpacing

                                        Kirigami.Icon {
                                            Layout.rightMargin: Kirigami.Units.largeSpacing
                                            implicitWidth: Kirigami.Units.iconSizes.smallMedium
                                            implicitHeight: Kirigami.Units.iconSizes.smallMedium
                                            source: 'weather-freezing-rain-day'
                                        }
                                        ColumnLayout {
                                            Layout.fillWidth: true
                                            spacing: Kirigami.Units.smallSpacing

                                            Label {
                                                Layout.fillWidth: true
                                                text: i18n('Dynamic')
                                                wrapMode: Text.Wrap
                                            }
                                            Label {
                                                Layout.fillWidth: true
                                                color: Kirigami.Theme.disabledTextColor
                                                wrapMode: Text.Wrap
                                                text: i18n('Animated and full-color backgrounds for weather forecasts.')
                                            }
                                        }
                                        Kirigami.Icon {
                                            implicitWidth: Kirigami.Units.iconSizes.smallMedium
                                            implicitHeight: Kirigami.Units.iconSizes.smallMedium
                                            visible: settingsModel.forecastStyle === "Dynamic"
                                            source: 'checkmark'
                                        }
                                    }
                                }
                                FormCard.AbstractFormDelegate {
                                    id: flatStyleDelegate
                                    onClicked: settingsModel.forecastStyle = "Flat"

                                    contentItem: RowLayout {
                                        spacing: Kirigami.Units.largeSpacing
                                        Kirigami.Icon {
                                            Layout.rightMargin: Kirigami.Units.largeSpacing
                                            implicitWidth: Kirigami.Units.iconSizes.smallMedium
                                            implicitHeight: Kirigami.Units.iconSizes.smallMedium
                                            source: 'weather-freezing-rain-day-symbolic'
                                        }
                                        ColumnLayout {
                                            Layout.fillWidth: true
                                            spacing: Kirigami.Units.smallSpacing

                                            Label {
                                                Layout.fillWidth: true
                                                text: i18n('Flat')
                                                wrapMode: Text.Wrap
                                            }
                                            Label {
                                                Layout.fillWidth: true
                                                color: Kirigami.Theme.disabledTextColor
                                                wrapMode: Text.Wrap
                                                text: i18n('Flat and simple backgrounds for weather forecasts.')
                                            }
                                        }
                                        Kirigami.Icon {
                                            implicitWidth: Kirigami.Units.iconSizes.smallMedium
                                            implicitHeight: Kirigami.Units.iconSizes.smallMedium
                                            visible: settingsModel.forecastStyle === "Flat"
                                            source: 'checkmark'
                                        }
                                    }
                                }
                            }
                        }
                    }

                    // Units page
                    Item {
                        ColumnLayout {
                            anchors.fill: parent
                            anchors.topMargin: Kirigami.Units.gridUnit
                            anchors.bottomMargin: Kirigami.Units.gridUnit + swipeViewFooter.height

                            FormCard.FormCard {
                                maximumWidth: root.cardWidth
                                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                                Layout.fillWidth: true

                                TemperatureUnitComboBox {}
                            }
                        }
                    }

                    // Add location page
                    Item {
                        ColumnLayout {
                            anchors.fill: parent
                            anchors.topMargin: Kirigami.Units.gridUnit
                            anchors.bottomMargin: Kirigami.Units.gridUnit + swipeViewFooter.height

                            FormCard.FormCard {
                                maximumWidth: root.cardWidth
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                AddLocationListView {
                                    id: addLocationListView
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                    clip: true
                                    onCloseRequested: root.requestNextPage()
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
