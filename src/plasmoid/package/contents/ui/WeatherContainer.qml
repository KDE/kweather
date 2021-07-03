import QtQuick 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.4
import org.kde.plasma.plasmoid 2.0
import org.kde.kirigami 2.11 as Kirigami
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
                text: plasmoid.nativeInterface.location
                color: Kirigami.Theme.textColor
                leftPadding: Kirigami.Units.smallSpacing
                font.bold: true
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
            }
            Label {
                id: temperatureLabel
                text: plasmoid.nativeInterface.temp + "°"
                color: Kirigami.Theme.activeTextColor
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
            }
        }
        Kirigami.Icon {
            source: plasmoid.nativeInterface.weatherIcon
        }

        Label {
            text: plasmoid.nativeInterface.desc
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
                text: i18n("%1%", plasmoid.nativeInterface.humidity)
                color: Kirigami.Theme.textColor
                font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.5
            }
        }

        RowLayout {
            visible: plasmoid.nativeInterface.precipitation > 0.01
            Kirigami.Icon {
                source: "raindrop"
                Layout.preferredHeight: Kirigami.Units.iconSizes.medium
                Layout.preferredWidth: Kirigami.Units.iconSizes.medium
            }
            Label {
                text: i18n("%1mm", plasmoid.nativeInterface.precipitation.toFixed(1))
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
                                    text: plasmoid.nativeInterface.location
                                }
                                Kirigami.Icon {
                                    source: plasmoid.nativeInterface.hourlyModel.currentIcon
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
                                    onClicked: plasmoid.nativeInterface.hourlyModel.openKWeather()
                                }
                            }
                            // weather header
                            ColumnLayout {
                                id: headerText
                                Label {
                                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 3
                                    font.weight: Font.Light
                                    text: plasmoid.nativeInterface.hourlyModel.currentTemperature + "°"
                                }
                                Label {
                                    font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
                                    font.weight: Font.Bold
                                    text: plasmoid.nativeInterface.hourlyModel.currentDescription
                                }
                            }
                        }
                        ListView {
                            Layout.preferredHeight: Kirigami.Units.gridUnit * 16
                            Layout.fillWidth: true
                            model: plasmoid.nativeInterface.hourlyModel
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
