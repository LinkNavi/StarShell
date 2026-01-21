cmake_minimum_required(VERSION 3.16)
project(starview-dock VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Qml)
find_package(LayerShellQt REQUIRED)

# Add the include directory BEFORE defining the executable
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files - headers AND sources together for MOC
qt_add_executable(starview-dock 
    include/DesktopFileParser.h
    include/WindowTracker.h
    include/Config.h
    src/main.cpp
    src/DesktopFileParser.cpp
    src/WindowTracker.cpp
    src/Config.cpp
)

# QML files
qt_add_qml_module(starview-dock
    URI StarViewDock
    VERSION 1.0
    QML_FILES 
        src/Main.qml
        src/AppLauncher.qml
)

# Link libraries
target_link_libraries(starview-dock PRIVATE
    Qt6::Core 
    Qt6::Gui 
    Qt6::Quick
    Qt6::Qml
    LayerShellQt::Interface
)
