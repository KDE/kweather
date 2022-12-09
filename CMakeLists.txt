#
# SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
# SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
#
# SPDX-License-Identifier: GPL-2.0-or-later
#
cmake_minimum_required(VERSION 3.16)

project(kweather)

set(PROJECT_VERSION "22.11")
set(QT_MIN_VERSION "5.15.2")
set(KF5_MIN_VERSION "5.101.0")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_PLASMOID "Build the weather plasmoid" ON)

include(FeatureSummary)

find_package(ECM ${KF5_MIN_VERSION} REQUIRED)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMPoQmTools)
include(ECMCheckOutboundLicense)
include(ECMFindQmlModule)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDEGitCommitHooks)
include(KDEClangFormat)

# clang-format
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES src/*.cpp src/*.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX KWEATHER
    VERSION_HEADER ${CMAKE_CURRENT_BINARY_DIR}/version.h
)

find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS
    Core
    Quick
    Test
    Gui
    Svg
    QuickControls2
    Charts
)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Config
    Kirigami2
    I18n
    CoreAddons
    Notifications
)
find_package(KF5KWeatherCore 0.6.0 REQUIRED)
find_package(Qt${QT_MAJOR_VERSION}QuickCompiler)
if (${QT_MAJOR_VERSION} EQUAL "6")
    find_package(Qt${QT_MAJOR_VERSION} REQUIRED COMPONENTS OpenGL)
endif()

find_package(KF5KirigamiAddons 0.6 REQUIRED)

if (ANDROID)
    find_package(OpenSSL REQUIRED)
else ()
    find_package(Qt${QT_MAJOR_VERSION} ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets)
endif()

if (NOT ANDROID AND BUILD_PLASMOID)
   find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS Plasma)
endif()

ecm_find_qmlmodule(org.kde.kirigamiaddons.labs.mobileform 0.1)
ecm_find_qmlmodule(org.kde.kholidays 1.0)

add_subdirectory(src)

ki18n_install(po)

if (ANDROID)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/android/version.gradle.in ${CMAKE_BINARY_DIR}/version.gradle)
endif()

install(PROGRAMS org.kde.kweather.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kweather.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(FILES org.kde.kweather.svg DESTINATION ${KDE_INSTALL_FULL_ICONDIR}/hicolor/scalable/apps/)

# DBus
configure_file(org.kde.kweather.service.in
               ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kweather.service)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kweather.service
        DESTINATION ${KDE_INSTALL_DBUSSERVICEDIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h *.qml)
ecm_check_outbound_license(LICENSES GPL-2.0-only GPL-3.0-only FILES ${ALL_SOURCE_FILES})

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
