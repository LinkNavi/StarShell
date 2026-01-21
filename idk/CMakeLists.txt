cmake_minimum_required(VERSION 3.16)

project(starview-panel VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Quick Qml)
find_package(LayerShellQt REQUIRED)

qt_standard_project_setup()

qt_add_executable(starview-panel
    src/main.cpp
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
    LayerShellQt::Interface
)

install(TARGETS starview-panel
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_qml_app_script(
    TARGET starview-panel
    OUTPUT_SCRIPT deploy_script
    MACOS_BUNDLE_POST_BUILD
    NO_UNSUPPORTED_PLATFORM_ERROR
    DEPLOY_USER_QML_MODULES_ON_UNSUPPORTED_PLATFORM
)

install(SCRIPT ${deploy_script})
