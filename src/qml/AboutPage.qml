/*
 * Copyright 2021 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.11 as Kirigami

Kirigami.AboutPage {
    id: aboutPage
    aboutData: {
        "displayName": i18n("Weather"),
        "productName": "kirigami/weather",
        "componentName": "kweather",
        "shortDescription": i18n("A convergent weather application for Plasma."),
        "homepage": "",
        "bugAddress": "",
        "version": "0.4",
        "otherText": "",
        "copyrightStatement": i18n("© 2020-2021 Plasma Development Team"),
        "desktopFileName": "org.kde.kweather",
        "authors": [
            {
                "name": i18n("Han Young"),
                "emailAddress": "hanyoung@protonmail.com",
            },
            {
                "name": i18n("Devin Lin"),
                "emailAddress": "espidev@gmail.com",
                "webAddress": "https://espi.dev"
            }
        ],
        "licenses": [
            {
                "name": "GPL v2",
                "text": "long, boring, license text",
                "spdx": "GPL-v2.0",
            }
        ]
    }
} 
