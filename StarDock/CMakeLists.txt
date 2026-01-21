cmake_minimum_required(VERSION 3.16)
project(starview-dock VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick)
find_package(LayerShellQt REQUIRED)

qt_add_executable(starview-dock src/main.cpp)

qt_add_qml_module(starview-dock
    URI StarViewDock
    VERSION 1.0
    QML_FILES src/Main.qml
)

target_link_libraries(starview-dock PRIVATE
    Qt6::Core 
    Qt6::Gui 
    Qt6::Quick
    LayerShellQt::Interface
)
