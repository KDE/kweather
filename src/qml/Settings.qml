import QtQuick 2.11
import Qt.labs.settings 1.0 as QtSettings
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.11 as Kirigami

QtObject {
    id: settings

    property string temperatureUnits: "Celsius"

    // persist settings
    property QtSettings.Settings _globalSettings: QtSettings.Settings {
        category: "Global"
        property alias temperatureUnits: settings.temperatureUnits
    }
}
