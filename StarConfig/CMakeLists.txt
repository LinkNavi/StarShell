cmake_minimum_required(VERSION 3.16)
project(starview-config VERSION 1.0 LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Quick Qml)
qt_standard_project_setup()

qt_add_executable(starview-config
    src/main.cpp
    src/ConfigManager.cpp
    src/ConfigManager.h
    src/WallpaperManager.cpp
    src/WallpaperManager.h
    src/MonitorManager.cpp
    src/MonitorManager.h
    src/ColorProvider.cpp
    src/ColorProvider.h
    src/NetworkManager.cpp
    src/NetworkManager.h
    src/PanelConfigWriter.h
    src/PanelConfigWriter.cpp
    src/module_server.h
    src/module_server.cpp
    include/toml.h
    src/toml.c
)

qt_add_qml_module(starview-config
    URI StarViewConfig
    VERSION 1.0
    QML_FILES
        src/Main.qml
        src/pages/GeneralPage.qml
        src/pages/DecorationPage.qml
        src/pages/AnimationPage.qml
        src/pages/TilingPage.qml
        src/pages/PanelPage.qml
        src/pages/ModulesPage.qml
        src/pages/WallpaperPage.qml
        src/pages/MonitorsPage.qml
        src/pages/NetworkPage.qml
        src/pages/KeybindingsPage.qml
        src/pages/RulesPage.qml
        src/pages/AutostartPage.qml
        src/components/SettingsGroup.qml
        src/components/SettingsRow.qml
        src/components/StyledComboBox.qml
)

target_include_directories(starview-config PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(starview-config PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
)

install(TARGETS starview-config
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
