import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import QtQuick.Shapes 1.12
import org.kde.kirigami 2.11 as Kirigami
import kweather 1.0

Rectangle {
    implicitWidth: Kirigami.Units.gridUnit * 5
    implicitHeight: Kirigami.Units.gridUnit * 8
    color: "transparent"

    property WeatherHour weather

    // actual hour display
    ColumnLayout {
        id: hourElement
        anchors.fill: parent
        anchors.leftMargin: Kirigami.Units.largeSpacing
        anchors.topMargin: Kirigami.Units.largeSpacing
        anchors.rightMargin: Kirigami.Units.largeSpacing
        anchors.bottomMargin: Kirigami.Units.largeSpacing
        spacing: Kirigami.Units.smallSpacing

        Kirigami.Icon {
            source: weather != null ? weather.weatherIcon : ""
            Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 3.5
            Layout.minimumWidth: Layout.minimumHeight
        }
        Label {
            text: weather != null ? weather.temperature : ""
            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1.3
        }
        Label {
            text: weather != null ? weather.weatherDescription : ""
        }

        // precipitation
        RowLayout {
            visible: weather != null
            Kirigami.Icon {
                source: "raindrop"
                Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                Layout.minimumWidth: Layout.minimumHeight
            }
            Label {
                color: Kirigami.Theme.disabledTextColor
                text: weather != null ? weather.precipitation.toFixed(1) + "mm" : ""
            }
        }

        // wind
        RowLayout {
            Kirigami.Icon {
                source: "arrow-right"
                Layout.minimumHeight: Kirigami.Theme.defaultFont.pointSize * 2
                Layout.minimumWidth: Layout.minimumHeight
            }
            Label {
                color: Kirigami.Theme.disabledTextColor
                text: weather != null ? weather.windSpeed.toFixed(1) + "km/h" : ""
            }
        }

        Label {
            font.weight: Font.Bold
            font.pointSize: Kirigami.Theme.defaultFont.pointSize * 1
            text: weather != null ? weather.date.toLocaleString(Qt.locale(), "h ap").replace(".", "").replace(".", "") : ""
        }
    }
}
