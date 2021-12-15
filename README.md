<!--
- Copyright 2020 Han Young <hanyoung@protonmail.com>
- Copyright 2020-2021 Devin Lin <devin@kde.org>
- SPDX-License-Identifier: GPL-2.0-or-later
-->

# KWeather <img src="kweather.svg" width="40"/>

<a href='https://flathub.org/apps/details/org.kde.kweather'><img width='190px' alt='Download on Flathub' src='https://flathub.org/assets/badges/flathub-badge-i-en.png'/></a>

A convergent weather application for Plasma.

## Features
* Daily and hourly weather data that can be viewed in flat and dynamic views
* Plasmoid that can be used on desktop and mobile

## Links
* Project page: https://invent.kde.org/plasma-mobile/kweather
* Issues: https://invent.kde.org/plasma-mobile/kweather/-/issues
* Development channel: https://matrix.to/#/#plasmamobile:matrix.org

## Dependencies
* KWeatherCore (https://invent.kde.org/libraries/kweathercore)
* Kirigami
* Qt Quick Controls
* Qt Quick Shapes

## Installing
```
mkdir build
cd build
cmake .. # add -DCMAKE_BUILD_TYPE=Release to compile for release
make
sudo make install
```

## Online APIs used
* api.met.no - Weather data, sunrise/sunset data
* geonames.org - Coordinates -> Timezone
* geoip.ubuntu.com - IP -> Coordinates

## Gallery

<img src="screenshots/kweather-mobile-dynamic.png" width="200px">
<img src="screenshots/kweather-mobile-dynamic2.png" width="200px">
<img src="screenshots/kweather-mobile-flat.png" width="200px">
<img src="screenshots/kweather-desktop-dynamic.png" width="500px">
