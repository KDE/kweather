import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.2
import org.kde.kirigami 2.11 as Kirigami

Kirigami.ApplicationWindow
{
    id: appwindow
    title: i18n("Weather")
    visible: true
    
    width: Kirigami.Units.gridUnit * 30
    height: Kirigami.Units.gridUnit * 45

    pageStack.initialPage: forecastPage
    
    function switchToPage(page) {
        while (pageStack.depth > 0) pageStack.pop();
        pageStack.push(page);
    }
    
    globalDrawer: Kirigami.GlobalDrawer {
        title: "Weather"
        bannerImageSource: "qrc:/resources/autumn.jpg"
        
        bannerVisible: true
//         modal: !wideScreen
//         width: 200
        
        actions: [
            Kirigami.Action {
                text: "Forecast"
                iconName: "weather-clear"
                onTriggered: switchToPage(forecastPage);
            },
            Kirigami.Action {
                text: "Locations"
                iconName: "globe"
                onTriggered: switchToPage(citiesPage);
            },
            Kirigami.Action {
                text: "Settings"
                iconName: "settings-configure"
                onTriggered: switchToPage(settingsPage);
            }
        ]
    }
    
    ForecastContainerPage {
        id: forecastPage
    }
    
    CitiesPage {
        id: citiesPage
        visible: false
    }
    
    SettingsPage {
        id: settingsPage
        visible: false
    }

    AddCityPage {
        id: addCityPage
        visible: false
    }

    Kirigami.AboutPage {
        id: aboutPage
        visible: false
        aboutData: {
            "displayName": "Weather",
            "productName": "kirigami/weather",
            "componentName": "kweather",
            "shortDescription": "A mobile friendly weather app built with Kirigami.",
            "homepage": "",
            "bugAddress": "",
            "version": "0.1",
            "otherText": "",
            "copyrightStatement": "© 2020 Plasma Development Team",
            "desktopFileName": "org.kde.kweather",
        }
    }
    
}
