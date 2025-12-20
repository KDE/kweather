// SPDX-FileCopyrightText: 2025 Devin Lin <devin@kde.org>
// SPDX-License-Identifier: LGPL-2.0-or-later

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls as QQC2

import org.kde.kcmutils
import org.kde.kirigami as Kirigami

import org.kde.kirigamiaddons.formcard as FormCard
import org.kde.plasma.plasmoid
import org.kde.kcmutils as KCMUtils

KCMUtils.ScrollViewKCM {
    id: root

    topPadding: 0
    bottomPadding: 0
    leftPadding: 0
    rightPadding: 0

    header: ColumnLayout {
        FormCard.FormCard {
            Layout.fillWidth: true
            Layout.topMargin: Kirigami.Units.largeSpacing
            Layout.bottomMargin: Kirigami.Units.largeSpacing

            FormCard.FormTextDelegate {
                text: i18n("Current location")
                description: Plasmoid.locationName ? Plasmoid.locationName : i18n("None")
            }
        }
    }

    view: ListView {
        id: listView

        header: ColumnLayout {
            width: listView.width
            spacing: 0

            Kirigami.SearchField {
                id: searchField
                Layout.fillWidth: true
                Layout.margins: Kirigami.Units.largeSpacing
                placeholderText: i18n("Search for a location…")
                onTextChanged: {
                    Plasmoid.queryModel.textChanged(text);
                }
                onEditingFinished: {
                    Plasmoid.queryModel.textChanged(text, 0); // when return is pressed, query immediately
                }
            }
        }

        model: Plasmoid.queryModel

        delegate: QQC2.ItemDelegate {
            width: listView.width
            text: model.name
            onClicked: Plasmoid.setLocation(model.name, model.latitude, model.longitude)
        }

        Kirigami.PlaceholderMessage {
            visible: !listView.model.loading && listView.count === 0
            anchors.centerIn: parent
            text: i18n("Set the location…")
            icon.name: "globe-symbolic"
        }

        QQC2.BusyIndicator {
            anchors.centerIn: parent
            running: listView.model.loading && listView.count === 0
            width: Kirigami.Units.iconSizes.huge
            height: width
        }
    }
}
