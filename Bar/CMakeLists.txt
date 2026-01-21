cmake_minimum_required(VERSION 3.16)

project(starview-panel VERSION 1.0 LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Quick Qml Network)
find_package(LayerShellQt REQUIRED)

qt_standard_project_setup()

qt_add_executable(starview-panel
    src/main.cpp
    src/ipc_handler.cpp
    src/ipc_handler.h
)

qt_add_qml_module(starview-panel
    URI StarViewPanel
    VERSION 1.0
    QML_FILES
        src/Main.qml
        src/WorkspaceButton.qml
)

target_link_libraries(starview-panel PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
    Qt6::Network
    LayerShellQt::Interface
)

install(TARGETS starview-panel
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
