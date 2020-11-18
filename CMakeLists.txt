#
# Copyright 2020 Han Young <hanyoung@protonmail.com>
# Copyright 2020 Devin Lin <espidev@gmail.com>
#
# SPDX-License-Identifier: GPL-2.0-or-later
#

project(kweather)

cmake_minimum_required(VERSION 2.8.12)
set(KF5_MIN_VERSION "5.77.0") // KQuickCharts
if (ANDROID)
    set(QT_MIN_VERSION "5.15.0") # the QTimezone doesn't work with Qt version before 5.15 beta 3
else()
    set(QT_MIN_VERSION "5.12.0")
endif()

include(FeatureSummary)

################# set KDE specific information #################

find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMPoQmTools)
include(KDECompilerSettings NO_POLICY_SCOPE)

################# Find dependencies #################

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS
    Core
    Quick
    Test
    Gui
    Svg
    QuickControls2
)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Config
    Kirigami2
    I18n
    CoreAddons
    Notifications
    QuickCharts
)

if (ANDROID)
   find_package(Qt5 ${QT_MIN_VERSION} REQUIRED COMPONENTS Svg)
   find_package(OpenSSL REQUIRED)
else()
   find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS Plasma)
endif()

################# build and install #################
add_subdirectory(src)

install(PROGRAMS org.kde.kweather.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kweather.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(FILES kweather.svg DESTINATION ${KDE_INSTALL_FULL_ICONDIR}/hicolor/scalable/apps/)

configure_file(org.kde.kweather.service.in
               ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kweather.service)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kweather.service
        DESTINATION ${KDE_INSTALL_DBUSSERVICEDIR})


feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
# add clang-format target for all our real source files
include(KDEClangFormat)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES src/*.cpp src/*.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
