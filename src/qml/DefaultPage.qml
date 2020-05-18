/*
 * Copyright 2020 Han Young <hanyoung@protonmail.com>
 * Copyright 2020 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.12 as Kirigami

// page shown if there are no weather locations configured
Kirigami.ScrollablePage {
    title: "Forecast"

    ListView { // empty list view to centre placeholdermessage
        Kirigami.PlaceholderMessage {
            anchors.centerIn: parent
            anchors.margins: Kirigami.Units.largeSpacing

            icon.name: "globe"
            text: i18n("No locations configured")

            helpfulAction: Kirigami.Action {
                iconName: "list-add"
                text: i18n("Add current location")
                onTriggered: {weatherLocationListModel.requestCurrentLocation()
                    switchToPage(forecastPage)
                }
            }
        }
    }
}
