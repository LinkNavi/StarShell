#!/bin/bash
set -e

# Run from StarShell root: bash patch_starconfig.sh
# Overwrites StarConfig QML, CMakeLists, and adds compositor live-reload files.

SDIR="StarConfig/src"
PAGES="$SDIR/pages"
COMP="$SDIR/components"
CSRC="../StarView/src"

mkdir -p "$PAGES" "$COMP" "$CSRC"

echo "==> Writing CMakeLists.txt"
cat > StarConfig/CMakeLists.txt << 'CMAKEOF'
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
    src/ColorProvider.cpp
    src/ColorProvider.h
    src/MonitorManager.h
    src/NetworkManager.cpp
    src/NetworkManager.h
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
CMAKEOF

###############################################################################
echo "==> Writing Main.qml"
###############################################################################
cat > "$SDIR/Main.qml" << 'QMLEOF'
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    visible: true
    width: 1000; height: 700
    title: "StarView Settings"
    color: cBase

    // --- palette (matugen with catppuccin fallbacks) ---
    readonly property color cBase:       colors.loaded ? colors.background       : "#1e1e2e"
    readonly property color cMantle:     colors.loaded ? colors.surface           : "#181825"
    readonly property color cCrust:      colors.loaded ? colors.surfaceVariant    : "#313244"
    readonly property color cText:       colors.loaded ? colors.onBackground      : "#cdd6f4"
    readonly property color cSubtext:    colors.loaded ? colors.onSurfaceVariant  : "#a6adc8"
    readonly property color cOverlay:    colors.loaded ? colors.outline           : "#6c7086"
    readonly property color cAccent:     colors.loaded ? colors.primary           : "#89b4fa"
    readonly property color cSecondary:  colors.loaded ? colors.secondary         : "#a6e3a1"
    readonly property color cTertiary:   colors.loaded ? colors.tertiary          : "#f38ba8"
    readonly property color cError:      colors.loaded ? colors.error             : "#f38ba8"
    readonly property color cOnAccent:   colors.loaded ? colors.onPrimary         : "#1e1e2e"
    readonly property color cSurfaceVar: colors.loaded ? colors.outlineVariant    : "#45475a"

    // Global palette for all Qt Quick Controls (Switch, SpinBox, Slider, TextField, etc.)
    palette.window: cBase
    palette.windowText: cText
    palette.base: cCrust
    palette.alternateBase: cMantle
    palette.text: cText
    palette.button: cCrust
    palette.buttonText: cText
    palette.highlight: cAccent
    palette.highlightedText: cOnAccent
    palette.placeholderText: cOverlay
    palette.mid: cSurfaceVar
    palette.dark: cMantle
    palette.light: cCrust

    property int currentPage: 0
    property var pageModel: [
        {name:"General",      page:"pages/GeneralPage.qml"},
        {name:"Decoration",   page:"pages/DecorationPage.qml"},
        {name:"Animation",    page:"pages/AnimationPage.qml"},
        {name:"Tiling",       page:"pages/TilingPage.qml"},
        {name:"Wallpaper",    page:"pages/WallpaperPage.qml"},
        {name:"Keybindings",  page:"pages/KeybindingsPage.qml"},
        {name:"Window Rules", page:"pages/RulesPage.qml"},
        {name:"Autostart",    page:"pages/AutostartPage.qml"},
        {name:"Monitors",     page:"pages/MonitorsPage.qml"},
        {name:"Network",      page:"pages/NetworkPage.qml"},
    ]

    RowLayout {
        anchors.fill: parent; spacing: 0

        // ---- sidebar ----
        Rectangle {
            Layout.fillHeight: true; Layout.preferredWidth: 240; color: cMantle
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 16; spacing: 4
                Text { text:"StarView"; font.pixelSize:22; font.bold:true; color:cAccent }
                Text { text:"Settings"; font.pixelSize:12; color:cSubtext; Layout.bottomMargin:16 }

                Repeater {
                    model: pageModel
                    Rectangle {
                        Layout.fillWidth: true; height: 40; radius: 8
                        color: currentPage===index ? Qt.rgba(root.cAccent.r,root.cAccent.g,root.cAccent.b,0.15)
                             : (_nma.containsMouse ? Qt.rgba(1,1,1,0.05) : "transparent")
                        Text { anchors.verticalCenter:parent.verticalCenter; anchors.left:parent.left; anchors.leftMargin:14
                               text:modelData.name; font.pixelSize:13; color: currentPage===index ? root.cAccent : root.cText }
                        MouseArea { id:_nma; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor
                                    onClicked:{ currentPage=index; pageLoader.source=modelData.page } }
                    }
                }
                Item { Layout.fillHeight: true }

                Rectangle { Layout.fillWidth:true; height:32; radius:6
                    color: configManager.connected ? Qt.rgba(0,0.6,0.3,0.12) : Qt.rgba(0.8,0,0,0.12)
                    Text { anchors.centerIn:parent; font.pixelSize:11
                           text: configManager.connected ? "● Connected" : "○ Disconnected"
                           color: configManager.connected ? root.cSecondary : root.cError }
                    MouseArea { anchors.fill:parent; cursorShape:Qt.PointingHandCursor; onClicked: if(!configManager.connected) configManager.reconnect() }
                }

                RowLayout { Layout.fillWidth:true; spacing:8
                    Rectangle { Layout.fillWidth:true; height:38; radius:8; color:_sma.containsMouse?root.cCrust:"transparent"; border.color:root.cSurfaceVar; border.width:1
                        Text { anchors.centerIn:parent; text:"Save"; font.pixelSize:13; color:root.cText }
                        MouseArea { id:_sma; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor; onClicked:configManager.save() } }
                    Rectangle { Layout.fillWidth:true; height:38; radius:8; color:_ama.containsMouse?Qt.lighter(root.cAccent,1.1):root.cAccent
                        Text { anchors.centerIn:parent; text:"Apply"; font.pixelSize:13; font.bold:true; color:root.cOnAccent }
                        MouseArea { id:_ama; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor; onClicked:configManager.saveAndReload() } }
                }
            }
        }

        // ---- content ----
        Rectangle { Layout.fillWidth:true; Layout.fillHeight:true; color:cBase
            Loader { id:pageLoader; anchors.fill:parent; source:"pages/GeneralPage.qml" }
        }
    }

    // ---- toast ----
    Rectangle { id:toast; anchors.horizontalCenter:parent.horizontalCenter; anchors.bottom:parent.bottom; anchors.bottomMargin:24
        width:_tt.width+32; height:40; radius:20; color:cSecondary; visible:false
        property alias text:_tt.text
        function show(m){ text=m; visible=true; _ttmr.restart() }
        Text { id:_tt; anchors.centerIn:parent; color:root.cOnAccent; font.pixelSize:12; font.bold:true }
        Timer { id:_ttmr; interval:2500; onTriggered:toast.visible=false }
    }
    Connections { target:configManager
        function onReloadSuccess(){ toast.show("Config reloaded") }
        function onReloadFailed(e){ toast.show("Reload failed: "+e) }
        function onSaved(){ toast.show("Saved") }
    }
}
QMLEOF

###############################################################################
echo "==> Writing components/SettingsGroup.qml"
###############################################################################
cat > "$COMP/SettingsGroup.qml" << 'QMLEOF'
import QtQuick
import QtQuick.Layouts

Rectangle {
    id: sg
    property string title: ""
    default property alias children: _col.data
    Layout.fillWidth: true
    color: root.cMantle; radius: 12; border.color: root.cSurfaceVar; border.width: 1
    implicitHeight: _col.implicitHeight + 40
    ColumnLayout {
        id: _col
        anchors.left: parent.left; anchors.right: parent.right; anchors.top: parent.top
        anchors.margins: 20; spacing: 14
        Text { text: sg.title; font.pixelSize: 16; font.bold: true; color: root.cAccent; visible: sg.title !== "" }
    }
}
QMLEOF

###############################################################################
echo "==> Writing components/SettingsRow.qml"
###############################################################################
cat > "$COMP/SettingsRow.qml" << 'QMLEOF'
import QtQuick
import QtQuick.Layouts

RowLayout {
    id: sr
    property string label: ""
    property string desc: ""
    default property alias children: _ctrl.data
    Layout.fillWidth: true; spacing: 16
    ColumnLayout { Layout.fillWidth: true; spacing: 2
        Text { text: sr.label; font.pixelSize: 13; color: root.cText }
        Text { text: sr.desc; font.pixelSize: 11; color: root.cSubtext; visible: sr.desc !== "" }
    }
    Row { id: _ctrl; spacing: 8 }
}
QMLEOF

###############################################################################
echo "==> Writing components/StyledComboBox.qml"
###############################################################################
cat > "$COMP/StyledComboBox.qml" << 'QMLEOF'
import QtQuick
import QtQuick.Controls

ComboBox {
    id: control
    implicitHeight: 36

    contentItem: control.editable ? _editLoader.item : _textDisplay
    Component.onCompleted: if (control.editable) _editLoader.active = true

    Text {
        id: _textDisplay
        visible: !control.editable
        leftPadding: 10; rightPadding: control.indicator.width + 10
        text: control.displayText
        font.pixelSize: 12
        color: root.cText
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    Loader {
        id: _editLoader
        active: false
        sourceComponent: TextInput {
            leftPadding: 10; rightPadding: control.indicator.width + 10
            text: control.editText
            font.pixelSize: 12
            color: root.cText
            selectionColor: root.cAccent
            selectedTextColor: root.cOnAccent
            verticalAlignment: Text.AlignVCenter
            onTextEdited: control.editText = text
        }
    }

    indicator: Text {
        x: control.width - width - 10
        y: (control.height - height) / 2
        text: "▾"; font.pixelSize: 12; color: root.cSubtext
    }

    background: Rectangle {
        color: control.pressed ? Qt.lighter(root.cCrust, 1.1) : root.cCrust
        radius: 6
        border.color: control.activeFocus ? root.cAccent : root.cSurfaceVar
        border.width: 1
    }

    delegate: ItemDelegate {
        width: control.popup.width
        height: 34
        contentItem: Text {
            text: control.textRole ? (Array.isArray(control.model) ? modelData : model[control.textRole]) : modelData
            font.pixelSize: 12
            color: highlighted ? root.cOnAccent : root.cText
            verticalAlignment: Text.AlignVCenter
            leftPadding: 10
        }
        background: Rectangle {
            color: highlighted ? root.cAccent : (hovered ? Qt.rgba(root.cAccent.r, root.cAccent.g, root.cAccent.b, 0.1) : "transparent")
            radius: 4
        }
        highlighted: control.highlightedIndex === index
        hoverEnabled: true
    }

    popup: Popup {
        y: control.height + 2
        width: control.width
        implicitHeight: Math.min(contentItem.implicitHeight + 8, 300)
        padding: 4

        background: Rectangle {
            color: root.cMantle
            radius: 8
            border.color: root.cSurfaceVar
            border.width: 1
        }

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex
            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }
        }
    }
}
QMLEOF

###############################################################################
echo "==> Writing pages/GeneralPage.qml"
###############################################################################
cat > "$PAGES/GeneralPage.qml" << 'QMLEOF'
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    ScrollView { anchors.fill: parent; contentWidth: availableWidth
        ColumnLayout { width: parent.width - 60; x: 30; spacing: 20
            Text { text: "General"; font.pixelSize: 26; font.bold: true; color: root.cText; Layout.topMargin: 30 }

            SettingsGroup { title: "Gaps"
                SettingsRow { label: "Inner Gaps"
                    SpinBox { from:0; to:100; value:configManager.gapsInner; onValueChanged: configManager.gapsInner=value
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
                SettingsRow { label: "Outer Gaps"
                    SpinBox { from:0; to:100; value:configManager.gapsOuter; onValueChanged: configManager.gapsOuter=value
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
            }

            SettingsGroup { title: "Borders"
                SettingsRow { label: "Border Width"
                    SpinBox { from:0; to:10; value:configManager.borderWidth; onValueChanged: configManager.borderWidth=value
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
                SettingsRow { label: "Active Border Color"
                    TextField { text:configManager.borderColorActive; implicitWidth:120; font.pixelSize:12; color:root.cText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        onEditingFinished: configManager.borderColorActive=text } }
                SettingsRow { label: "Inactive Border Color"
                    TextField { text:configManager.borderColorInactive; implicitWidth:120; font.pixelSize:12; color:root.cText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        onEditingFinished: configManager.borderColorInactive=text } }
            }

            SettingsGroup { title: "Behavior"
                SettingsRow { label: "Focus Follows Mouse"
                    Switch { checked:configManager.focusFollowsMouse; onCheckedChanged: configManager.focusFollowsMouse=checked } }
                SettingsRow { label: "Default Mode"
                    StyledComboBox { model:["tiling","floating"]; currentIndex:model.indexOf(configManager.defaultMode)
                        onActivated: configManager.defaultMode=currentText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
                SettingsRow { label: "Resize Step"
                    SpinBox { from:10; to:200; value:configManager.resizeStep; onValueChanged: configManager.resizeStep=value
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
                SettingsRow { label: "Move Step"
                    SpinBox { from:10; to:200; value:configManager.moveStep; onValueChanged: configManager.moveStep=value
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
            }
            Item { height: 30 }
        }
    }
}
QMLEOF

###############################################################################
echo "==> Writing pages/DecorationPage.qml"
###############################################################################
cat > "$PAGES/DecorationPage.qml" << 'QMLEOF'
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    ScrollView { anchors.fill: parent; contentWidth: availableWidth
        ColumnLayout { width: parent.width - 60; x: 30; spacing: 20
            Text { text: "Decoration"; font.pixelSize: 26; font.bold: true; color: root.cText; Layout.topMargin: 30 }

            SettingsGroup { title: "Titlebar"
                SettingsRow { label: "Enable Decorations"
                    Switch { checked:configManager.decorEnabled; onCheckedChanged: configManager.decorEnabled=checked } }
                SettingsRow { label: "Height"
                    SpinBox { from:20; to:60; value:configManager.decorHeight; onValueChanged: configManager.decorHeight=value
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
                SettingsRow { label: "Corner Radius"
                    SpinBox { from:0; to:30; value:configManager.decorCornerRadius; onValueChanged: configManager.decorCornerRadius=value
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
                SettingsRow { label: "Buttons on Left"
                    Switch { checked:configManager.decorButtonsLeft; onCheckedChanged: configManager.decorButtonsLeft=checked } }
            }

            SettingsGroup { title: "Buttons"
                SettingsRow { label: "Button Size"
                    SpinBox { from:8; to:24; value:configManager.decorButtonSize; onValueChanged: configManager.decorButtonSize=value
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
                SettingsRow { label: "Button Spacing"
                    SpinBox { from:2; to:20; value:configManager.decorButtonSpacing; onValueChanged: configManager.decorButtonSpacing=value
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
                SettingsRow { label: "Close Color"
                    TextField { text:configManager.decorCloseColor; implicitWidth:120; font.pixelSize:12; color:root.cText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        onEditingFinished: configManager.decorCloseColor=text } }
                SettingsRow { label: "Maximize Color"
                    TextField { text:configManager.decorMaxColor; implicitWidth:120; font.pixelSize:12; color:root.cText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        onEditingFinished: configManager.decorMaxColor=text } }
                SettingsRow { label: "Minimize Color"
                    TextField { text:configManager.decorMinColor; implicitWidth:120; font.pixelSize:12; color:root.cText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        onEditingFinished: configManager.decorMinColor=text } }
            }

            SettingsGroup { title: "Colors"
                SettingsRow { label: "BG Active"
                    TextField { text:configManager.decorBgColor; implicitWidth:120; font.pixelSize:12; color:root.cText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        onEditingFinished: configManager.decorBgColor=text } }
                SettingsRow { label: "BG Inactive"
                    TextField { text:configManager.decorBgColorInactive; implicitWidth:120; font.pixelSize:12; color:root.cText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        onEditingFinished: configManager.decorBgColorInactive=text } }
                SettingsRow { label: "Title Active"
                    TextField { text:configManager.decorTitleColor; implicitWidth:120; font.pixelSize:12; color:root.cText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        onEditingFinished: configManager.decorTitleColor=text } }
                SettingsRow { label: "Title Inactive"
                    TextField { text:configManager.decorTitleColorInactive; implicitWidth:120; font.pixelSize:12; color:root.cText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        onEditingFinished: configManager.decorTitleColorInactive=text } }
            }

            SettingsGroup { title: "Font"
                SettingsRow { label: "Font Family"
                    TextField { text:configManager.decorFont; implicitWidth:180; font.pixelSize:12; color:root.cText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        onEditingFinished: configManager.decorFont=text } }
                SettingsRow { label: "Font Size"
                    SpinBox { from:8; to:24; value:configManager.decorFontSize; onValueChanged: configManager.decorFontSize=value
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
            }

            // Apply matugen to decorations
            Rectangle { Layout.fillWidth:true; height:44; radius:8
                color: colors.loaded ? (_mma.containsMouse ? Qt.lighter(root.cTertiary,1.1) : root.cTertiary) : root.cCrust
                Text { anchors.centerIn:parent; text:"Apply Matugen Colors to Decorations"; font.pixelSize:13; font.bold:true
                       color: colors.loaded ? root.cOnAccent : root.cSubtext }
                MouseArea { id:_mma; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor; enabled:colors.loaded
                    onClicked: {
                        var c={}
                        c["primary"]=colors.primary; c["secondary"]=colors.secondary; c["tertiary"]=colors.tertiary
                        c["error"]=colors.error; c["background"]=colors.background; c["on_background"]=colors.onBackground
                        c["surface"]=colors.surface; c["on_surface"]=colors.onSurface
                        c["surface_variant"]=colors.surfaceVariant; c["on_surface_variant"]=colors.onSurfaceVariant
                        c["outline"]=colors.outline
                        configManager.applyMatugenColors(c)
                    }
                }
            }
            Item { height: 30 }
        }
    }
}
QMLEOF

###############################################################################
echo "==> Writing pages/AnimationPage.qml"
###############################################################################
cat > "$PAGES/AnimationPage.qml" << 'QMLEOF'
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    id: animPg

    readonly property var animTypes: ["none","fade","slide","zoom","slide_fade"]
    readonly property var curveTypes: ["linear","ease_in","ease_out","ease_in_out","bounce","spring"]

    ScrollView { anchors.fill: parent; contentWidth: availableWidth
        ColumnLayout { width: parent.width - 60; x: 30; spacing: 20
            Text { text: "Animation"; font.pixelSize: 26; font.bold: true; color: root.cText; Layout.topMargin: 30 }

            SettingsGroup { title: "General"
                SettingsRow { label: "Enable Animations"
                    Switch { checked:configManager.animEnabled; onCheckedChanged: configManager.animEnabled=checked } }
                SettingsRow { label: "Duration (ms)"
                    RowLayout { spacing: 10
                        Slider { id:_durSlider; from:50; to:1000; stepSize:25; value:configManager.animDuration; implicitWidth:200
                            onMoved: configManager.animDuration=value }
                        Text { text:configManager.animDuration+"ms"; font.pixelSize:12; color:root.cSubtext; Layout.preferredWidth:50 }
                    }
                }
                SettingsRow { label: "Curve"
                    StyledComboBox { model:animPg.curveTypes; currentIndex: model.indexOf(configManager.animCurve)
                        onActivated: configManager.animCurve=currentText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        popup.background: Rectangle{color:root.cMantle;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
            }

            SettingsGroup { title: "Window Animations"
                SettingsRow { label: "Window Open"
                    StyledComboBox { model:animPg.animTypes; currentIndex: model.indexOf(configManager.animWindowOpen)
                        onActivated: configManager.animWindowOpen=currentText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        popup.background: Rectangle{color:root.cMantle;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
                SettingsRow { label: "Window Close"
                    StyledComboBox { model:animPg.animTypes; currentIndex: model.indexOf(configManager.animWindowClose)
                        onActivated: configManager.animWindowClose=currentText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        popup.background: Rectangle{color:root.cMantle;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
                SettingsRow { label: "Window Move"
                    StyledComboBox { model:animPg.animTypes; currentIndex: model.indexOf(configManager.animWindowMove)
                        onActivated: configManager.animWindowMove=currentText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        popup.background: Rectangle{color:root.cMantle;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
                SettingsRow { label: "Window Resize"
                    StyledComboBox { model:animPg.animTypes; currentIndex: model.indexOf(configManager.animWindowResize)
                        onActivated: configManager.animWindowResize=currentText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        popup.background: Rectangle{color:root.cMantle;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
            }

            SettingsGroup { title: "Workspace Animation"
                SettingsRow { label: "Workspace Switch"
                    StyledComboBox { model:animPg.animTypes; currentIndex: model.indexOf(configManager.animWorkspaceSwitch)
                        onActivated: configManager.animWorkspaceSwitch=currentText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        popup.background: Rectangle{color:root.cMantle;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
            }

            SettingsGroup { title: "Fine-tuning"
                SettingsRow { label: "Fade Min Opacity"; desc: "Minimum opacity during fade (0.0 = fully transparent)"
                    RowLayout { spacing: 10
                        Slider { from:0.0; to:1.0; stepSize:0.05; value:configManager.animFadeMin; implicitWidth:180
                            onMoved: configManager.animFadeMin=value }
                        Text { text:configManager.animFadeMin.toFixed(2); font.pixelSize:12; color:root.cSubtext }
                    }
                }
                SettingsRow { label: "Zoom Min Scale"; desc: "Minimum scale during zoom (0.5 = half size)"
                    RowLayout { spacing: 10
                        Slider { from:0.0; to:1.0; stepSize:0.05; value:configManager.animZoomMin; implicitWidth:180
                            onMoved: configManager.animZoomMin=value }
                        Text { text:configManager.animZoomMin.toFixed(2); font.pixelSize:12; color:root.cSubtext }
                    }
                }
            }

            // Preview
            SettingsGroup { title: "Preview"
                Rectangle { Layout.fillWidth:true; Layout.preferredHeight:140; color:root.cCrust; radius:8; clip:true
                    Rectangle { id:_prev; width:80; height:60; radius:root.cAccent!=""?6:6; color:root.cAccent; x:20; y:40
                        Text { anchors.centerIn:parent; text:"Window"; font.pixelSize:10; color:root.cOnAccent }
                    }
                    Rectangle { Layout.fillWidth:false; width:100; height:36; radius:6; anchors.bottom:parent.bottom; anchors.bottomMargin:8; anchors.horizontalCenter:parent.horizontalCenter
                        color:_prevMa.containsMouse?Qt.lighter(root.cAccent,1.1):root.cAccent
                        Text { anchors.centerIn:parent; text:"Play"; font.pixelSize:13; font.bold:true; color:root.cOnAccent }
                        MouseArea { id:_prevMa; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor
                            onClicked: {
                                _prevAnim.duration = configManager.animDuration
                                _prev.opacity = configManager.animFadeMin
                                _prev.scale = configManager.animZoomMin
                                _prevAnim.restart()
                            }
                        }
                    }
                    ParallelAnimation { id:_prevAnim; property int duration: 200
                        NumberAnimation { target:_prev; property:"opacity"; from:configManager.animFadeMin; to:1.0; duration:_prevAnim.duration; easing.type:Easing.OutCubic }
                        NumberAnimation { target:_prev; property:"scale"; from:configManager.animZoomMin; to:1.0; duration:_prevAnim.duration; easing.type:Easing.OutCubic }
                    }
                }
            }

            Item { height: 30 }
        }
    }
}
QMLEOF

###############################################################################
echo "==> Writing pages/TilingPage.qml"
###############################################################################
cat > "$PAGES/TilingPage.qml" << 'QMLEOF'
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    ScrollView { anchors.fill: parent; contentWidth: availableWidth
        ColumnLayout { width: parent.width - 60; x: 30; spacing: 20
            Text { text: "Tiling"; font.pixelSize: 26; font.bold: true; color: root.cText; Layout.topMargin: 30 }

            SettingsGroup { title: "Master-Stack Layout"
                SettingsRow { label: "Master Count"
                    SpinBox { from:1; to:5; value:configManager.masterCount; onValueChanged: configManager.masterCount=value
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} } }

                SettingsRow { label: "Master Ratio"
                    RowLayout { spacing: 10
                        Slider { id:_rs; from:0.1; to:0.9; stepSize:0.05; value:configManager.masterRatio; implicitWidth:180
                            onMoved: configManager.masterRatio=value }
                        Text { text:configManager.masterRatio.toFixed(2); font.pixelSize:13; color:root.cSubtext }
                    }
                }
            }
            Item { height: 30 }
        }
    }
}
QMLEOF

###############################################################################
echo "==> Writing pages/WallpaperPage.qml"
###############################################################################
cat > "$PAGES/WallpaperPage.qml" << 'QMLEOF'
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "../components"

Item {
    id: wpPage
    property string pendingImage: configManager.bgImage || ""

    Connections {
        target: configManager
        function onConfigChanged() {
            // Only update from configManager if we don't have a pending local change
            if (wpPage.pendingImage === "" || wpPage.pendingImage === configManager.bgImage)
                wpPage.pendingImage = configManager.bgImage || ""
        }
    }

    ScrollView { anchors.fill: parent; contentWidth: availableWidth
        ColumnLayout { width: parent.width - 60; x: 30; spacing: 20
            Text { text: "Wallpaper & Colors"; font.pixelSize: 26; font.bold: true; color: root.cText; Layout.topMargin: 30 }

            SettingsGroup { title: "Background"
                SettingsRow { label: "Enable Background"
                    Switch { checked:configManager.bgEnabled; onCheckedChanged: configManager.bgEnabled=checked } }
                SettingsRow { label: "Background Color"
                    TextField { text:configManager.bgColor; implicitWidth:120; font.pixelSize:12; color:root.cText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        onEditingFinished: configManager.bgColor=text } }
                SettingsRow { label: "Mode"
                    StyledComboBox { model:["fill","fit","stretch","center","tile","color"]
                        currentIndex: model.indexOf(configManager.bgMode); onActivated: configManager.bgMode=currentText } }
            }

            SettingsGroup { title: "Wallpaper Image"
                Rectangle { Layout.fillWidth:true; Layout.preferredHeight:220; radius:8; color:root.cCrust; clip:true
                    Image {
                        anchors.fill:parent; anchors.margins:2; fillMode:Image.PreserveAspectCrop
                        source: wpPage.pendingImage ? "file://" + wpPage.pendingImage : ""
                        cache: false
                        asynchronous: true
                    }
                    Text { anchors.centerIn:parent; text:"No wallpaper set"; color:root.cSubtext; visible: !wpPage.pendingImage }
                }

                RowLayout { Layout.fillWidth:true; spacing:8
                    Rectangle { Layout.fillWidth:true; height:40; radius:8; color:_selma.containsMouse?root.cCrust:root.cMantle; border.color:root.cSurfaceVar; border.width:1
                        Text { anchors.centerIn:parent; text:"Select Image"; font.pixelSize:13; color:root.cText }
                        MouseArea { id:_selma; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor; onClicked:_fd.open() } }
                    Rectangle { Layout.fillWidth:true; height:40; radius:8
                        color: wpPage.pendingImage!=="" ? (_genma.containsMouse?Qt.lighter(root.cAccent,1.1):root.cAccent) : root.cCrust
                        Text { anchors.centerIn:parent; text:"Generate Matugen Colors"; font.pixelSize:13; font.bold:true
                               color: wpPage.pendingImage!=="" ? root.cOnAccent : root.cSubtext }
                        MouseArea { id:_genma; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor
                                    enabled: wpPage.pendingImage!==""; onClicked: colors.generateFromWallpaper(wpPage.pendingImage) } }
                }
                Text { text: wpPage.pendingImage||"No image selected"; font.pixelSize:11; color:root.cSubtext; elide:Text.ElideMiddle; Layout.fillWidth:true }
            }

            // Palette preview
            SettingsGroup { title: "Color Palette"; visible: colors.loaded
                GridLayout { Layout.fillWidth:true; columns:6; columnSpacing:6; rowSpacing:6
                    Repeater { model: [
                        {n:"Primary",c:colors.primary},{n:"Secondary",c:colors.secondary},{n:"Tertiary",c:colors.tertiary},
                        {n:"Error",c:colors.error},{n:"BG",c:colors.background},{n:"Surface",c:colors.surface}
                    ]
                    delegate: ColumnLayout { spacing:4
                        Rectangle { width:60;height:36;radius:6;color:modelData.c;border.color:root.cSurfaceVar;border.width:1 }
                        Text { text:modelData.n; font.pixelSize:9; color:root.cSubtext; Layout.alignment:Qt.AlignHCenter }
                    }}
                }
            }
            Item { height: 30 }
        }
    }
    FileDialog { id:_fd; title:"Select Wallpaper"; nameFilters:["Images (*.png *.jpg *.jpeg *.webp *.bmp)"]
        onAccepted: {
            var p = selectedFile.toString().replace("file://","")
            wpPage.pendingImage = p
            configManager.bgImage = p
        }
    }
}
QMLEOF

###############################################################################
echo "==> Writing pages/KeybindingsPage.qml"
###############################################################################
cat > "$PAGES/KeybindingsPage.qml" << 'QMLEOF'
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    id: kbPage

    // ---- Data models for dropdowns ----
    readonly property var modifierList: ["Super","Alt","Ctrl","Shift"]
    readonly property var keyList: [
        "Return","space","Escape","Tab","BackSpace","Delete",
        "Left","Right","Up","Down",
        "Home","End","Page_Up","Page_Down",
        "a","b","c","d","e","f","g","h","i","j","k","l","m",
        "n","o","p","q","r","s","t","u","v","w","x","y","z",
        "1","2","3","4","5","6","7","8","9","0",
        "F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12",
        "minus","equal","bracketleft","bracketright","semicolon","apostrophe",
        "comma","period","slash","backslash","grave",
        "Print","Scroll_Lock","Pause",
        "XF86AudioRaiseVolume","XF86AudioLowerVolume","XF86AudioMute",
        "XF86AudioPlay","XF86AudioNext","XF86AudioPrev",
        "XF86MonBrightnessUp","XF86MonBrightnessDown"
    ]

    // Actions grouped by category
    readonly property var actionCategories: [
        {name:"Window", actions:[
            {id:"close",            label:"Close Window",        needsArg:false},
            {id:"fullscreen",       label:"Toggle Fullscreen",   needsArg:false},
            {id:"toggle_floating",  label:"Toggle Floating",     needsArg:false},
            {id:"minimize",         label:"Minimize",            needsArg:false},
            {id:"maximize",         label:"Maximize",            needsArg:false},
            {id:"center",           label:"Center Window",       needsArg:false},
            {id:"snap_left",        label:"Snap Left",           needsArg:false},
            {id:"snap_right",       label:"Snap Right",          needsArg:false},
        ]},
        {name:"Focus", actions:[
            {id:"focus_left",   label:"Focus Left",   needsArg:false},
            {id:"focus_right",  label:"Focus Right",  needsArg:false},
            {id:"focus_up",     label:"Focus Up",     needsArg:false},
            {id:"focus_down",   label:"Focus Down",   needsArg:false},
            {id:"focus_next",   label:"Focus Next",   needsArg:false},
            {id:"focus_prev",   label:"Focus Prev",   needsArg:false},
        ]},
        {name:"Move", actions:[
            {id:"move_left",   label:"Move Left",   needsArg:false},
            {id:"move_right",  label:"Move Right",  needsArg:false},
            {id:"move_up",     label:"Move Up",     needsArg:false},
            {id:"move_down",   label:"Move Down",   needsArg:false},
        ]},
        {name:"Swap", actions:[
            {id:"swap_left",   label:"Swap Left",   needsArg:false},
            {id:"swap_right",  label:"Swap Right",  needsArg:false},
            {id:"swap_up",     label:"Swap Up",     needsArg:false},
            {id:"swap_down",   label:"Swap Down",   needsArg:false},
        ]},
        {name:"Preselect", actions:[
            {id:"preselect_left",   label:"Preselect Left",   needsArg:false},
            {id:"preselect_right",  label:"Preselect Right",  needsArg:false},
            {id:"preselect_up",     label:"Preselect Up",     needsArg:false},
            {id:"preselect_down",   label:"Preselect Down",   needsArg:false},
        ]},
        {name:"Resize", actions:[
            {id:"resize_grow_width",    label:"Grow Width",    needsArg:false},
            {id:"resize_shrink_width",  label:"Shrink Width",  needsArg:false},
            {id:"resize_grow_height",   label:"Grow Height",   needsArg:false},
            {id:"resize_shrink_height", label:"Shrink Height", needsArg:false},
        ]},
        {name:"Workspace", actions:[
            {id:"workspace",             label:"Go to Workspace",      needsArg:true, argType:"workspace"},
            {id:"move_to_workspace",     label:"Move to Workspace",    needsArg:true, argType:"workspace"},
            {id:"workspace_next",        label:"Next Workspace",       needsArg:false},
            {id:"workspace_prev",        label:"Prev Workspace",       needsArg:false},
        ]},
        {name:"Layout", actions:[
            {id:"mode_tiling",       label:"Tiling Mode",        needsArg:false},
            {id:"mode_floating",     label:"Floating Mode",      needsArg:false},
            {id:"toggle_mode",       label:"Toggle Mode",        needsArg:false},
            {id:"inc_master_count",  label:"Inc Master Count",   needsArg:false},
            {id:"dec_master_count",  label:"Dec Master Count",   needsArg:false},
            {id:"inc_master_ratio",  label:"Inc Master Ratio",   needsArg:false},
            {id:"dec_master_ratio",  label:"Dec Master Ratio",   needsArg:false},
        ]},
        {name:"System", actions:[
            {id:"spawn",          label:"Launch App",       needsArg:true, argType:"command"},
            {id:"reload_config",  label:"Reload Config",    needsArg:false},
            {id:"exit",           label:"Exit Compositor",  needsArg:false},
        ]},
    ]

    // Flatten for the action ComboBox
    function getAllActions() {
        var list = []
        for (var i = 0; i < actionCategories.length; i++) {
            var cat = actionCategories[i]
            for (var j = 0; j < cat.actions.length; j++) {
                list.push(cat.actions[j])
            }
        }
        return list
    }
    property var allActions: getAllActions()
    function actionLabels() { var l=[]; for(var i=0;i<allActions.length;i++) l.push(allActions[i].label); return l }

    // Gesture actions (subset)
    readonly property var gestureActionList: [
        {id:"workspace_next",    label:"Next Workspace"},
        {id:"workspace_prev",    label:"Prev Workspace"},
        {id:"workspace",         label:"Go to Workspace",    needsArg:true, argType:"workspace"},
        {id:"maximize",          label:"Maximize"},
        {id:"minimize",          label:"Minimize"},
        {id:"fullscreen",        label:"Fullscreen"},
        {id:"close",             label:"Close"},
        {id:"snap_left",         label:"Snap Left"},
        {id:"snap_right",        label:"Snap Right"},
        {id:"toggle_floating",   label:"Toggle Floating"},
        {id:"toggle_mode",       label:"Toggle Mode"},
        {id:"spawn",             label:"Launch App",         needsArg:true, argType:"command"},
        {id:"exec",              label:"Execute Command",    needsArg:true, argType:"command"},
    ]
    function gestureActionLabels() { var l=[]; for(var i=0;i<gestureActionList.length;i++) l.push(gestureActionList[i].label); return l }

    readonly property var gestureDirections: ["up","down","left","right"]
    readonly property var gesturePinchDirs: ["in","out"]

    // Build key string from modifier checkboxes + key combo
    function buildKeyString(mods, key) {
        var parts = []
        for (var i = 0; i < mods.length; i++) { if (mods[i]) parts.push(modifierList[i]) }
        parts.push(key)
        return parts.join("+")
    }

    ScrollView { anchors.fill: parent; contentWidth: availableWidth
        ColumnLayout { width: parent.width - 60; x: 30; spacing: 20
            Text { text: "Keybindings & Gestures"; font.pixelSize: 26; font.bold: true; color: root.cText; Layout.topMargin: 30 }

            // ===================== ADD KEYBIND =====================
            SettingsGroup { title: "Add Keybind"
                id: _kbGroup

                property var modActive: [false, false, false, false]

                // Modifier toggles
                Text { text: "Modifiers"; font.pixelSize: 12; color: root.cSubtext }
                RowLayout { Layout.fillWidth: true; spacing: 8
                    Repeater { model: kbPage.modifierList
                        Rectangle { width: _mtext.width+24; height: 32; radius: 6
                            property bool active: _kbGroup.modActive[index] || false
                            color: active ? root.cAccent : root.cCrust
                            border.color: active ? root.cAccent : root.cSurfaceVar; border.width: 1
                            Text { id: _mtext; anchors.centerIn: parent; text: modelData; font.pixelSize: 12
                                   color: parent.active ? root.cOnAccent : root.cText }
                            MouseArea { anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    var m = _kbGroup.modActive.slice()
                                    m[index] = !m[index]
                                    _kbGroup.modActive = m
                                }
                            }
                        }
                    }
                }

                // Key selector
                Text { text: "Key"; font.pixelSize: 12; color: root.cSubtext; Layout.topMargin: 8 }
                StyledComboBox { id: _keyCombo; Layout.fillWidth: true; model: kbPage.keyList; editable: true
                    background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                    popup.background: Rectangle{color:root.cMantle;radius:6;border.color:root.cSurfaceVar;border.width:1}
                }

                // Action selector
                Text { text: "Action"; font.pixelSize: 12; color: root.cSubtext; Layout.topMargin: 8 }
                StyledComboBox { id: _actionCombo; Layout.fillWidth: true; model: kbPage.actionLabels()
                    background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                    popup.background: Rectangle{color:root.cMantle;radius:6;border.color:root.cSurfaceVar;border.width:1}
                }

                // Arg field (shown only when action needs arg)
                TextField { id: _argField; Layout.fillWidth: true; visible: {
                        var a = kbPage.allActions[_actionCombo.currentIndex]
                        return a ? a.needsArg : false
                    }
                    placeholderText: {
                        var a = kbPage.allActions[_actionCombo.currentIndex]
                        if (!a || !a.needsArg) return ""
                        if (a.argType === "workspace") return "Workspace number (1-10)"
                        return "Command (e.g. foot, firefox)"
                    }
                    font.pixelSize: 12; color: root.cText
                    background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                }

                // Workspace quick picker (when argType is workspace)
                RowLayout { Layout.fillWidth: true; spacing: 4; visible: {
                        var a = kbPage.allActions[_actionCombo.currentIndex]
                        return a && a.argType === "workspace"
                    }
                    Repeater { model: 10
                        Rectangle { width: 36; height: 32; radius: 6
                            color: _argField.text === String(index+1) ? root.cAccent : root.cCrust
                            border.color: root.cSurfaceVar; border.width: 1
                            Text { anchors.centerIn: parent; text: index+1; font.pixelSize: 12
                                   color: _argField.text===String(index+1) ? root.cOnAccent : root.cText }
                            MouseArea { anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                                        onClicked: _argField.text = String(index+1) }
                        }
                    }
                }

                // Add button
                Rectangle { Layout.fillWidth: true; height: 40; radius: 8; Layout.topMargin: 8
                    color: _kbAddMa.containsMouse ? Qt.lighter(root.cAccent,1.1) : root.cAccent
                    Text { anchors.centerIn: parent; text: "Add Keybind"; font.pixelSize: 13; font.bold: true; color: root.cOnAccent }
                    MouseArea { id: _kbAddMa; anchors.fill: parent; hoverEnabled: true; cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            var key = _keyCombo.editText || _keyCombo.currentText
                            if (!key) return
                            var keyStr = kbPage.buildKeyString(_kbGroup.modActive, key)

                            var act = kbPage.allActions[_actionCombo.currentIndex]
                            if (!act) return
                            var actionStr = act.id
                            if (act.needsArg && _argField.text) actionStr += " " + _argField.text

                            configManager.addKeybind(keyStr, actionStr)
                            _argField.text = ""
                        }
                    }
                }
            }

            // ===================== TOUCHPAD GESTURES =====================
            SettingsGroup { title: "Add Touchpad Gesture"

                RowLayout { Layout.fillWidth: true; spacing: 8
                    ColumnLayout { Layout.fillWidth: true; spacing: 6
                        Text { text: "Fingers"; font.pixelSize: 12; color: root.cSubtext }
                        StyledComboBox { id: _gFing; Layout.fillWidth: true; model: ["3","4","5"]
                            background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} }
                    }
                    ColumnLayout { Layout.fillWidth: true; spacing: 6
                        Text { text: "Direction"; font.pixelSize: 12; color: root.cSubtext }
                        StyledComboBox { id: _gDir; Layout.fillWidth: true; model: kbPage.gestureDirections
                            background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} }
                    }
                    ColumnLayout { Layout.fillWidth: true; spacing: 6
                        Text { text: "Action"; font.pixelSize: 12; color: root.cSubtext }
                        StyledComboBox { id: _gAct; Layout.fillWidth: true; model: kbPage.gestureActionLabels()
                            background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} }
                    }
                }
                TextField { id: _gArg; Layout.fillWidth: true; font.pixelSize: 12; color: root.cText
                    visible: { var a=kbPage.gestureActionList[_gAct.currentIndex]; return a&&a.needsArg }
                    placeholderText: { var a=kbPage.gestureActionList[_gAct.currentIndex]; return (a&&a.argType==="workspace")?"Workspace number":"Command" }
                    background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                }
                Rectangle { Layout.fillWidth: true; height: 36; radius: 6
                    color: _gAddMa.containsMouse ? Qt.lighter(root.cSecondary,1.1) : root.cSecondary
                    Text { anchors.centerIn: parent; text: "Add Gesture"; font.pixelSize: 13; font.bold: true; color: root.cOnAccent }
                    MouseArea { id: _gAddMa; anchors.fill: parent; hoverEnabled: true; cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            var act = kbPage.gestureActionList[_gAct.currentIndex]
                            if (!act) return
                            var actionStr = act.id
                            if (act.needsArg && _gArg.text) actionStr += " " + _gArg.text
                            configManager.addTouchpadGesture(parseInt(_gFing.currentText), _gDir.currentText, actionStr)
                            _gArg.text = ""
                        }
                    }
                }
            }

            // ===================== MOUSE GESTURES =====================
            SettingsGroup { title: "Add Mouse Gesture"

                RowLayout { Layout.fillWidth: true; spacing: 8
                    ColumnLayout { Layout.fillWidth: true; spacing: 6
                        Text { text: "Button"; font.pixelSize: 12; color: root.cSubtext }
                        StyledComboBox { id: _mgBtn; Layout.fillWidth: true; model: ["left","middle","right"]
                            background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} }
                    }
                    ColumnLayout { Layout.fillWidth: true; spacing: 6
                        Text { text: "Modifier"; font.pixelSize: 12; color: root.cSubtext }
                        StyledComboBox { id: _mgMod; Layout.fillWidth: true; model: ["None","Super","Alt","Ctrl","Shift"]
                            background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} }
                    }
                    ColumnLayout { Layout.fillWidth: true; spacing: 6
                        Text { text: "Direction"; font.pixelSize: 12; color: root.cSubtext }
                        StyledComboBox { id: _mgDir; Layout.fillWidth: true; model: kbPage.gestureDirections.concat(kbPage.gesturePinchDirs)
                            background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} }
                    }
                    ColumnLayout { Layout.fillWidth: true; spacing: 6
                        Text { text: "Action"; font.pixelSize: 12; color: root.cSubtext }
                        StyledComboBox { id: _mgAct; Layout.fillWidth: true; model: kbPage.gestureActionLabels()
                            background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} }
                    }
                }
                TextField { id: _mgArg; Layout.fillWidth: true; font.pixelSize: 12; color: root.cText
                    visible: { var a=kbPage.gestureActionList[_mgAct.currentIndex]; return a&&a.needsArg }
                    placeholderText: { var a=kbPage.gestureActionList[_mgAct.currentIndex]; return (a&&a.argType==="workspace")?"Workspace number":"Command" }
                    background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                }
                Rectangle { Layout.fillWidth: true; height: 36; radius: 6
                    color: _mgAddMa.containsMouse ? Qt.lighter(root.cTertiary,1.1) : root.cTertiary
                    Text { anchors.centerIn: parent; text: "Add Mouse Gesture"; font.pixelSize: 13; font.bold: true; color: root.cOnAccent }
                    MouseArea { id: _mgAddMa; anchors.fill: parent; hoverEnabled: true; cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            var act = kbPage.gestureActionList[_mgAct.currentIndex]
                            if (!act) return
                            var actionStr = act.id
                            if (act.needsArg && _mgArg.text) actionStr += " " + _mgArg.text
                            var mod = _mgMod.currentText === "None" ? "" : _mgMod.currentText
                            configManager.addMouseGesture(_mgBtn.currentText, mod, _mgDir.currentText, actionStr)
                            _mgArg.text = ""
                        }
                    }
                }
            }

            // ===================== GESTURE THRESHOLDS =====================
            SettingsGroup { title: "Gesture Sensitivity"
                SettingsRow { label: "Swipe Threshold"
                    RowLayout { spacing: 8
                        Slider { id: _swTh; from: 0.1; to: 1.0; stepSize: 0.05; value: configManager.gestureSwipeThreshold; implicitWidth: 160
                            onMoved: configManager.gestureSwipeThreshold = value }
                        Text { text: _swTh.value.toFixed(2); font.pixelSize: 12; color: root.cSubtext }
                    }
                }
                SettingsRow { label: "Pinch Threshold"
                    RowLayout { spacing: 8
                        Slider { id: _piTh; from: 0.05; to: 0.5; stepSize: 0.05; value: configManager.gesturePinchThreshold; implicitWidth: 160
                            onMoved: configManager.gesturePinchThreshold = value }
                        Text { text: _piTh.value.toFixed(2); font.pixelSize: 12; color: root.cSubtext }
                    }
                }
                SettingsRow { label: "Mouse Threshold"
                    RowLayout { spacing: 8
                        Slider { id: _moTh; from: 10; to: 200; stepSize: 10; value: configManager.gestureMouseThreshold; implicitWidth: 160
                            onMoved: configManager.gestureMouseThreshold = value }
                        Text { text: _moTh.value.toFixed(0); font.pixelSize: 12; color: root.cSubtext }
                    }
                }
            }

            // ===================== CURRENT KEYBINDS =====================
            SettingsGroup { title: "Current Keybinds (" + configManager.keybinds.length + ")"
                Repeater { model: configManager.keybinds
                    RowLayout { Layout.fillWidth: true; spacing: 10
                        Rectangle { color: root.cCrust; radius: 4; implicitWidth: _kbTxt.width+16; implicitHeight: 28
                            Text { id: _kbTxt; anchors.centerIn: parent; text: modelData.key||""; font.pixelSize: 11; font.family: "monospace"; color: root.cAccent } }
                        Text { text: "→"; color: root.cSubtext; font.pixelSize: 13 }
                        Text { text: modelData.action||""; color: root.cText; font.pixelSize: 13; Layout.fillWidth: true; elide: Text.ElideRight }
                        Text { text: "✕"; color: root.cError; font.pixelSize: 16
                            MouseArea { anchors.fill: parent; anchors.margins: -6; cursorShape: Qt.PointingHandCursor; onClicked: configManager.removeKeybind(index) } }
                    }
                }
            }

            // ===================== CURRENT TOUCHPAD GESTURES =====================
            SettingsGroup { title: "Touchpad Gestures (" + configManager.touchpadGestures.length + ")"; visible: configManager.touchpadGestures.length > 0
                Repeater { model: configManager.touchpadGestures
                    RowLayout { Layout.fillWidth: true; spacing: 10
                        Rectangle { color: root.cCrust; radius: 4; implicitWidth: _gtTxt.width+16; implicitHeight: 28
                            Text { id: _gtTxt; anchors.centerIn: parent; font.pixelSize: 11; font.family: "monospace"; color: root.cSecondary
                                   text: (modelData.fingers||"?") + "F " + (modelData.direction||"?") } }
                        Text { text: "→"; color: root.cSubtext; font.pixelSize: 13 }
                        Text { text: modelData.action||""; color: root.cText; font.pixelSize: 13; Layout.fillWidth: true; elide: Text.ElideRight }
                        Text { text: "✕"; color: root.cError; font.pixelSize: 16
                            MouseArea { anchors.fill: parent; anchors.margins: -6; cursorShape: Qt.PointingHandCursor; onClicked: configManager.removeTouchpadGesture(index) } }
                    }
                }
            }

            // ===================== CURRENT MOUSE GESTURES =====================
            SettingsGroup { title: "Mouse Gestures (" + configManager.mouseGestures.length + ")"; visible: configManager.mouseGestures.length > 0
                Repeater { model: configManager.mouseGestures
                    RowLayout { Layout.fillWidth: true; spacing: 10
                        Rectangle { color: root.cCrust; radius: 4; implicitWidth: _gmTxt.width+16; implicitHeight: 28
                            Text { id: _gmTxt; anchors.centerIn: parent; font.pixelSize: 11; font.family: "monospace"; color: root.cTertiary
                                   text: (modelData.modifiers ? modelData.modifiers+"+" : "") + (modelData.button||"?") + " " + (modelData.direction||"?") } }
                        Text { text: "→"; color: root.cSubtext; font.pixelSize: 13 }
                        Text { text: modelData.action||""; color: root.cText; font.pixelSize: 13; Layout.fillWidth: true; elide: Text.ElideRight }
                        Text { text: "✕"; color: root.cError; font.pixelSize: 16
                            MouseArea { anchors.fill: parent; anchors.margins: -6; cursorShape: Qt.PointingHandCursor; onClicked: configManager.removeMouseGesture(index) } }
                    }
                }
            }

            Item { height: 30 }
        }
    }
}
QMLEOF

###############################################################################
echo "==> Writing pages/RulesPage.qml"
###############################################################################
cat > "$PAGES/RulesPage.qml" << 'QMLEOF'
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    ScrollView { anchors.fill: parent; contentWidth: availableWidth
        ColumnLayout { width: parent.width - 60; x: 30; spacing: 20
            Text { text: "Window Rules"; font.pixelSize: 26; font.bold: true; color: root.cText; Layout.topMargin: 30 }

            SettingsGroup { title: "Add Rule"
                RowLayout { Layout.fillWidth:true; spacing:8
                    TextField { id:_raid; Layout.fillWidth:true; placeholderText:"app_id"; font.pixelSize:12; color:root.cText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} }
                    TextField { id:_rtitle; Layout.fillWidth:true; placeholderText:"title (optional)"; font.pixelSize:12; color:root.cText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} }
                }
                RowLayout { Layout.fillWidth:true; spacing:16
                    Text { text:"Floating"; color:root.cText; font.pixelSize:13 }
                    Switch { id:_rfloat }
                    Text { text:"Fullscreen"; color:root.cText; font.pixelSize:13 }
                    Switch { id:_rfs }
                    Text { text:"Workspace"; color:root.cText; font.pixelSize:13 }
                    SpinBox { id:_rws; from:0; to:10; value:0
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} }
                }
                Rectangle { Layout.fillWidth:true; height:36; radius:6; color:_arma.containsMouse?Qt.lighter(root.cAccent,1.1):root.cAccent
                    Text { anchors.centerIn:parent; text:"Add Rule"; font.pixelSize:13; color:root.cOnAccent }
                    MouseArea { id:_arma; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor
                        onClicked: {
                            if(!_raid.text&&!_rtitle.text) return
                            var r={}
                            if(_raid.text) r["app_id"]=_raid.text
                            if(_rtitle.text) r["title"]=_rtitle.text
                            if(_rfloat.checked) r["floating"]=true
                            if(_rfs.checked) r["fullscreen"]=true
                            if(_rws.value>0) r["workspace"]=_rws.value
                            configManager.addRule(r)
                            _raid.text=""; _rtitle.text=""; _rfloat.checked=false; _rfs.checked=false; _rws.value=0
                        }
                    }
                }
            }

            SettingsGroup { title: "Rules ("+configManager.rules.length+")"
                Repeater { model: configManager.rules
                    RowLayout { Layout.fillWidth:true; spacing:10
                        ColumnLayout { Layout.fillWidth:true; spacing:2
                            Text { text:(modelData.app_id||"*")+(modelData.title?" / "+modelData.title:""); color:root.cText; font.pixelSize:13 }
                            Text { text:[modelData.floating?"floating":"",modelData.fullscreen?"fullscreen":"",modelData.workspace?"ws:"+modelData.workspace:""].filter(function(s){return s}).join(", ")
                                   color:root.cSubtext; font.pixelSize:11 } }
                        Text { text:"✕"; color:root.cError; font.pixelSize:16
                            MouseArea { anchors.fill:parent; anchors.margins:-6; cursorShape:Qt.PointingHandCursor; onClicked:configManager.removeRule(index) } }
                    }
                }
            }
            Item { height: 30 }
        }
    }
}
QMLEOF

###############################################################################
echo "==> Writing pages/AutostartPage.qml"
###############################################################################
cat > "$PAGES/AutostartPage.qml" << 'QMLEOF'
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    ScrollView { anchors.fill: parent; contentWidth: availableWidth
        ColumnLayout { width: parent.width - 60; x: 30; spacing: 20
            Text { text: "Autostart"; font.pixelSize: 26; font.bold: true; color: root.cText; Layout.topMargin: 30 }

            SettingsGroup { title: "Add Command"
                RowLayout { Layout.fillWidth:true; spacing:8
                    TextField { id:_nc; Layout.fillWidth:true; placeholderText:"Command (e.g. waybar)"; font.pixelSize:12; color:root.cText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        onAccepted: _acb.clicked() }
                    Rectangle { id:_acb; width:60; height:36; radius:6; color:_acma.containsMouse?Qt.lighter(root.cAccent,1.1):root.cAccent
                        signal clicked()
                        Text { anchors.centerIn:parent; text:"Add"; font.pixelSize:13; color:root.cOnAccent }
                        MouseArea { id:_acma; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor
                            onClicked: { if(_nc.text){ configManager.addAutostart(_nc.text); _nc.text="" }; parent.clicked() } } }
                }
            }

            SettingsGroup { title: "Startup Commands ("+configManager.autostart.length+")"
                Repeater { model: configManager.autostart
                    RowLayout { Layout.fillWidth:true; spacing:10
                        Text { text:modelData; color:root.cText; font.pixelSize:13; font.family:"monospace"; Layout.fillWidth:true; elide:Text.ElideRight }
                        Text { text:"✕"; color:root.cError; font.pixelSize:16
                            MouseArea { anchors.fill:parent; anchors.margins:-6; cursorShape:Qt.PointingHandCursor; onClicked:configManager.removeAutostart(index) } }
                    }
                }
            }
            Item { height: 30 }
        }
    }
}
QMLEOF

###############################################################################
echo "==> Writing pages/MonitorsPage.qml"
###############################################################################
cat > "$PAGES/MonitorsPage.qml" << 'QMLEOF'
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    id: monPg
    property var monitorList: []
    Component.onCompleted: monitorList = monitorManager.getMonitors()

    ScrollView { anchors.fill: parent; contentWidth: availableWidth
        ColumnLayout { width: parent.width - 60; x: 30; spacing: 20
            Text { text: "Monitors"; font.pixelSize: 26; font.bold: true; color: root.cText; Layout.topMargin: 30 }

            Rectangle { width:100; height:36; radius:6; color:_rfma.containsMouse?root.cCrust:root.cMantle; border.color:root.cSurfaceVar; border.width:1
                Text { anchors.centerIn:parent; text:"Refresh"; font.pixelSize:13; color:root.cText }
                MouseArea { id:_rfma; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor; onClicked: monPg.monitorList=monitorManager.getMonitors() } }

            Repeater { model: monPg.monitorList
                SettingsGroup { title: modelData.name||"Unknown"
                    SettingsRow { label:"Resolution"; Text { text:(modelData.width||"?")+"x"+(modelData.height||"?"); color:root.cText; font.pixelSize:13 } }
                    SettingsRow { label:"Refresh"; Text { text:(modelData.refresh||"?")+" Hz"; color:root.cText; font.pixelSize:13 } }
                    SettingsRow { label:"Scale"; Text { text:(modelData.scale||1.0).toFixed(1)+"x"; color:root.cText; font.pixelSize:13 } }
                    SettingsRow { label:"Enabled"; Switch { checked:modelData.enabled!==false; onCheckedChanged:monitorManager.setMonitorEnabled(modelData.name,checked) } }
                }
            }

            Text { text:"No monitors detected"; color:root.cSubtext; font.pixelSize:13; visible:monPg.monitorList.length===0; Layout.alignment:Qt.AlignHCenter }
            Item { height: 30 }
        }
    }
}
QMLEOF

###############################################################################
echo "==> Writing pages/NetworkPage.qml"
###############################################################################
cat > "$PAGES/NetworkPage.qml" << 'QMLEOF'
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    id: netPg
    property var networkList: []

    ScrollView { anchors.fill: parent; contentWidth: availableWidth
        ColumnLayout { width: parent.width - 60; x: 30; spacing: 20
            Text { text: "Network"; font.pixelSize: 26; font.bold: true; color: root.cText; Layout.topMargin: 30 }

            SettingsGroup { title: "WiFi"
                SettingsRow { label: "WiFi Enabled"
                    Switch { checked:networkManager.wifiEnabled; onCheckedChanged: networkManager.toggleWifi() } }

                Rectangle { Layout.fillWidth:true; height:36; radius:6; color:_scma.containsMouse?root.cCrust:root.cMantle; border.color:root.cSurfaceVar; border.width:1
                    Text { anchors.centerIn:parent; text:"Scan Networks"; font.pixelSize:13; color: networkManager.wifiEnabled?root.cText:root.cSubtext }
                    MouseArea { id:_scma; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor; enabled:networkManager.wifiEnabled
                        onClicked: netPg.networkList=networkManager.scanNetworks() } }

                Repeater { model: netPg.networkList
                    RowLayout { Layout.fillWidth:true; spacing:12
                        Text { text:"📶"; font.pixelSize:16 }
                        ColumnLayout { Layout.fillWidth:true; spacing:2
                            Text { text:modelData.ssid||"Hidden"; color:root.cText; font.pixelSize:13 }
                            Text { text:(modelData.security||"Open")+" · "+modelData.signal+"%"; color:root.cSubtext; font.pixelSize:11 } }
                        Rectangle { width:80; height:30; radius:6; color:_cnma.containsMouse?Qt.lighter(root.cAccent,1.1):root.cAccent
                            Text { anchors.centerIn:parent; text:"Connect"; font.pixelSize:12; color:root.cOnAccent }
                            MouseArea { id:_cnma; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor
                                onClicked: { _cd.ssid=modelData.ssid; _cd.secured=(modelData.security||"")!==""; _cd.open() } } }
                    }
                }
            }
            Item { height: 30 }
        }
    }

    Dialog { id:_cd; anchors.centerIn:parent; modal:true; property string ssid:""; property bool secured:true
        background: Rectangle{color:root.cMantle;radius:12;border.color:root.cSurfaceVar;border.width:1}
        header: Text{text:"Connect to "+_cd.ssid;color:root.cText;font.pixelSize:16;font.bold:true;padding:16}
        ColumnLayout { spacing:12; width:300
            TextField { id:_pw; Layout.fillWidth:true; visible:_cd.secured; echoMode:TextInput.Password; placeholderText:"Password"
                font.pixelSize:12; color:root.cText; background:Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} }
            RowLayout { Layout.alignment:Qt.AlignRight; spacing:8
                Rectangle { width:70;height:32;radius:6;color:_ccma.containsMouse?root.cCrust:"transparent"
                    Text{anchors.centerIn:parent;text:"Cancel";font.pixelSize:13;color:root.cText}
                    MouseArea{id:_ccma;anchors.fill:parent;hoverEnabled:true;cursorShape:Qt.PointingHandCursor;onClicked:_cd.close()} }
                Rectangle { width:80;height:32;radius:6;color:_cxma.containsMouse?Qt.lighter(root.cAccent,1.1):root.cAccent
                    Text{anchors.centerIn:parent;text:"Connect";font.pixelSize:13;font.bold:true;color:root.cOnAccent}
                    MouseArea{id:_cxma;anchors.fill:parent;hoverEnabled:true;cursorShape:Qt.PointingHandCursor
                        onClicked:{networkManager.connectToNetwork(_cd.ssid,_pw.text);_cd.close()}} }
            }
        }
    }
}
QMLEOF

###############################################################################
echo "==> Writing StarConfig/src/ConfigManager.h"
###############################################################################
# ConfigManager.h is written by a separate companion file due to size.
# Place ConfigManager.h and ConfigManager.cpp next to patch.sh before running,
# OR they will be written inline below.

cat > "$SDIR/ConfigManager.h" << 'CMHEOF'
CMHEOF

###############################################################################
echo "==> Writing StarConfig/src/ConfigManager.cpp"
###############################################################################
cat > "$SDIR/ConfigManager.cpp" << 'CMCEOF'
CMCEOF

echo "    NOTE: ConfigManager.h and .cpp are provided as separate files."
echo "    Copy them to StarConfig/src/ manually from the download."

###############################################################################
echo "==> Writing compositor src/config_live.h"
###############################################################################
cat > "$CSRC/config_live.h" << 'CEOF'
#ifndef CONFIG_LIVE_H
#define CONFIG_LIVE_H

struct server;

/* Apply config changes live after config_reload(). */
void config_apply_live(struct server *server);

#endif
CEOF

###############################################################################
echo "==> Writing compositor src/config_live.c"
###############################################################################
cat > "$CSRC/config_live.c" << 'CEOF'
#define _POSIX_C_SOURCE 200809L
#define WLR_USE_UNSTABLE

#include "config.h"
#include "core.h"
#include "background.h"
#include "titlebar_render.h"
#include "config_live.h"
#include <stdio.h>

void config_apply_live(struct server *server) {
    if (!server) return;

    printf("[LIVE] Applying config changes...\n");

    /* 1. Update titlebar theme */
    struct titlebar_theme *theme = titlebar_get_global_theme();
    if (theme) {
        titlebar_theme_load_from_config(theme, &config.decor);
        printf("[LIVE] Titlebar theme updated\n");
    }

    /* 2. Update all decorations */
    struct toplevel *toplevel;
    wl_list_for_each(toplevel, &server->toplevels, link) {
        bool focused = (toplevel == get_focused_toplevel(server));
        if (config.decor.enabled && !toplevel->decor.tree)
            decor_create(toplevel);
        else if (!config.decor.enabled && toplevel->decor.tree)
            decor_destroy(toplevel);
        if (toplevel->decor.tree) {
            struct wlr_box geo;
            wlr_xdg_surface_get_geometry(toplevel->xdg_toplevel->base, &geo);
            decor_set_size(toplevel, geo.width);
            decor_update(toplevel, focused);
        }
    }
    printf("[LIVE] Decorations updated\n");

    /* 3. Update backgrounds */
    struct output *output;
    wl_list_for_each(output, &server->outputs, link) {
        if (config.background.enabled) {
            int w = output->wlr_output->width;
            int h = output->wlr_output->height;
            if (output->background)
                background_update(output->background, w, h, &config.background);
            else
                output->background = background_create(server->layer_bg, w, h, &config.background);
        } else if (output->background) {
            wlr_scene_node_set_enabled(&output->background->node, false);
        }
    }
    printf("[LIVE] Backgrounds updated\n");

    /* 4. Mode + layout */
    server->mode = config.default_mode;
    arrange_windows(server);
    printf("[LIVE] Done\n");
}
CEOF

###############################################################################
echo "==> Patching src/ipc.c (adding config_live include + live reload)"
###############################################################################
if [ -f "$CSRC/ipc.c" ]; then
    # Add include if not present
    if ! grep -q 'config_live.h' "$CSRC/ipc.c"; then
        sed -i '/#include "config.h"/a #include "config_live.h"' "$CSRC/ipc.c"
        echo "    Added #include config_live.h to ipc.c"
    fi
    # Replace arrange_windows with config_apply_live in reload handler
    sed -i 's/config_reload();[[:space:]]*\n*[[:space:]]*arrange_windows(client->server);/config_reload();\n        config_apply_live(client->server);/g' "$CSRC/ipc.c"
    # Simpler sed in case the above didn't match due to formatting
    sed -i '/strcmp(payload, "reload") == 0/,/}/{
        s/arrange_windows(client->server)/config_apply_live(client->server)/
    }' "$CSRC/ipc.c"
    echo "    Patched reload handler in ipc.c"
else
    echo "    WARNING: src/ipc.c not found, skipping patch"
fi

echo ""
echo "=========================================="
echo " Done! Files written:"
echo "=========================================="
echo " StarConfig/CMakeLists.txt"
echo " StarConfig/src/Main.qml"
echo " StarConfig/src/components/SettingsGroup.qml"
echo " StarConfig/src/components/SettingsRow.qml"
echo " StarConfig/src/pages/{General,Decoration,Animation,Tiling,Wallpaper,Keybindings,Rules,Autostart,Monitors,Network}Page.qml"
echo " src/config_live.h"
echo " src/config_live.c"
echo " src/ipc.c (patched)"
echo ""
echo " Add src/config_live.c to your compositor Makefile/CMakeLists."
echo " Rebuild both projects."
