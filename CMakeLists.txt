#
# SPDX-FileCopyrightText: 2020 Han Young <hanyoung@protonmail.com>
# SPDX-FileCopyrightText: 2020 Devin Lin <espidev@gmail.com>
#
# SPDX-License-Identifier: GPL-2.0-or-later
#
cmake_minimum_required(VERSION 3.16)

project(kweather)

set(PROJECT_VERSION "21.08")
set(QT_MIN_VERSION "5.15.0")
set(KF5_MIN_VERSION "5.85.0")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_PLASMOID "Build the weather plasmoid" ON)

include(FeatureSummary)

find_package(ECM ${KF5_MIN_VERSION} REQUIRED)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(ECMPoQmTools)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMCheckOutboundLicense)
include(KDEGitCommitHooks)

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX KWEATHER
    VERSION_HEADER ${CMAKE_CURRENT_BINARY_DIR}/version.h
)

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS
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
find_package(KF5KWeatherCore 0.5.0 REQUIRED)

if (ANDROID)
   find_package(OpenSSL REQUIRED)
else ()
    find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets)
endif()

if (NOT ANDROID AND BUILD_PLASMOID)
   find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS Plasma)
endif()

add_subdirectory(src)

install(PROGRAMS org.kde.kweather.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kweather.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(FILES kweather.svg DESTINATION ${KDE_INSTALL_FULL_ICONDIR}/hicolor/scalable/apps/)

configure_file(org.kde.kweather.service.in
               ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kweather.service)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/org.kde.kweather.service
        DESTINATION ${KDE_INSTALL_DBUSSERVICEDIR})


feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
include(KDEClangFormat)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES src/*.cpp src/*.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h *.qml)
ecm_check_outbound_license(LICENSES GPL-2.0-only GPL-3.0-only FILES ${ALL_SOURCE_FILES})

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
