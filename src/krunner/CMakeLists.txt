#
# SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
# SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
# SPDX-FileCopyrightText: 2026 Arran Ubels <kde@arran4.com>
#
# SPDX-License-Identifier: GPL-2.0-or-later
#

kcoreaddons_add_plugin(krunnerkweather SOURCES kweatherrunner.cpp INSTALL_NAMESPACE "kf6/krunner")
target_link_libraries(krunnerkweather KF6::Runner KF6::I18n KF6::ConfigCore KWeatherCore Qt6::Network Qt6::Gui Qt6::Core)
