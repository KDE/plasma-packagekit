project(plasma-packagekit)
cmake_minimum_required(VERSION 2.8.12)

set(PROJECT_VERSION "5.9.90")
set(PROJECT_VERSION_MAJOR 5)

set(QT_MIN_VERSION "5.7.0")
set(ECM_MIN_VERSION "5.30.0")

find_package(ECM ${ECM_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMOptionalAddSubdirectory)

# We need DBus for packagekit, unfortunately it doesn't manage it properly.
find_package(Qt5 ${QT_MIN_VERSION} REQUIRED COMPONENTS DBus Quick)
find_package(KF5 REQUIRED COMPONENTS Plasma)
find_package(PackageKitQt5 REQUIRED)

add_subdirectory(applet)
add_subdirectory(src)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
