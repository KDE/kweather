#
# SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
# SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
#
# SPDX-License-Identifier: GPL-2.0-or-later
#
cmake_minimum_required(VERSION 3.16)

# KDE Applications version, managed by release script.
set(RELEASE_SERVICE_VERSION_MAJOR "24")
set(RELEASE_SERVICE_VERSION_MINOR "08")
set(RELEASE_SERVICE_VERSION_MICRO "3")
set(RELEASE_SERVICE_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project(kweather VERSION ${RELEASE_SERVICE_VERSION})

set(QT_MIN_VERSION "6.5.0")
set(KF_MIN_VERSION "5.240.0")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_PLASMOID "Build the weather plasmoid" ON)

include(FeatureSummary)

find_package(ECM ${KF_MIN_VERSION} REQUIRED)

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
include(ECMQmlModule)
include(ECMAddAndroidApk)

# clang-format
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES src/*.cpp src/*.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX KWEATHER
    VERSION_HEADER ${CMAKE_CURRENT_BINARY_DIR}/version.h
)

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS
    Core
    Quick
    Test
    Gui
    Svg
    QuickControls2
    Charts
    OpenGL
)
find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS
    Config
    Kirigami
    I18n
    CoreAddons
    Notifications
)
find_package(KWeatherCore 0.8.0 REQUIRED)

find_package(KF6KirigamiAddons 0.11 REQUIRED)

if (ANDROID)
    find_package(OpenSSL REQUIRED)
else ()
    find_package(Qt6 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets)
endif()

if (NOT ANDROID AND BUILD_PLASMOID)
   find_package(Plasma REQUIRED)
endif()

ecm_find_qmlmodule(org.kde.kholidays 1.0)
add_definitions(-DQT_NO_CONTEXTLESS_CONNECT)
add_subdirectory(src)

ki18n_install(po)

if (ANDROID)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/android/version.gradle.in ${CMAKE_BINARY_DIR}/version.gradle)
    install(FILES org.kde.kweather.svg DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kirigami/breeze-internal/icons/)
else()
    install(PROGRAMS org.kde.kweather.desktop DESTINATION ${KDE_INSTALL_APPDIR})
    install(FILES org.kde.kweather.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
    install(FILES org.kde.kweather.svg DESTINATION ${KDE_INSTALL_FULL_ICONDIR}/hicolor/scalable/apps/)

    # DBus
    configure_file(org.kde.kweather.service.in
                ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kweather.service)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kweather.service
            DESTINATION ${KDE_INSTALL_DBUSSERVICEDIR})
endif()

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

file(GLOB_RECURSE ALL_SOURCE_FILES src/*.cpp src/*.h src/*.qml)
ecm_check_outbound_license(LICENSES GPL-2.0-only GPL-3.0-only FILES ${ALL_SOURCE_FILES})

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
