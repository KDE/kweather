import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtQuick.Shapes 1.12
import org.kde.kirigami 2.11 as Kirigami

Kirigami.ScrollablePage {
    title: i18n("Settings")

    topPadding: 0
    bottomPadding: 0
    leftPadding: 0
    rightPadding: 0
    Kirigami.ColumnView.fillWidth: false

    ColumnLayout {
        spacing: 0

        ItemDelegate {
            Layout.fillWidth: true
            implicitHeight: Kirigami.Units.gridUnit * 3
            onClicked: temperatureUnits.open()

            ColumnLayout {
                spacing: -5
                anchors.leftMargin: Kirigami.Units.gridUnit
                anchors.rightMargin: Kirigami.Units.gridUnit
                anchors.fill: parent

                Label {
                    text: i18n("Temperature Units")
                    font.weight: Font.Bold
                }
                Label {
                    text: i18n(settingsModel.temperatureUnits)
                }
            }
        }

        Kirigami.Separator {
            Layout.fillWidth: true
        }

        ItemDelegate {
            Layout.fillWidth: true
            font.bold: true
            text: i18n("About")
            leftPadding: Kirigami.Units.gridUnit
            rightPadding: Kirigami.Units.gridUnit
            implicitHeight: Kirigami.Units.gridUnit * 3
            onClicked: pageStack.push(aboutPage)
        }

        Kirigami.Separator {
            Layout.fillWidth: true
        }
    }

    // temperature unit dialog
    Dialog {
        id: temperatureUnits
        modal: true
        focus: true
        x: (parent.width - width) / 2
        y: parent.height / 2 - height
        width: Math.min(parent.width - Kirigami.Units.gridUnit * 4, Kirigami.Units.gridUnit * 20)
        height: Kirigami.Units.gridUnit * 20
        standardButtons: Dialog.Close
        title: i18n("Temperature Units")

        onAccepted: weatherLocationListModel.updateUi();
        onRejected: weatherLocationListModel.updateUi();

        contentItem: ScrollView {
            ListView {
                model: ["Celsius", "Fahrenheit"]
                delegate: RadioDelegate {
                    width: parent.width
                    text: i18n(modelData)
                    checked: settingsModel.temperatureUnits == modelData
                    onCheckedChanged: {
                        if (checked) {
                            settingsModel.temperatureUnits = modelData;
                        }
                    }
                }
            }
            Component.onCompleted: background.visible = true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        }
    }
    
}
