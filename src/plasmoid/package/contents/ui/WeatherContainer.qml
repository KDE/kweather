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
}
