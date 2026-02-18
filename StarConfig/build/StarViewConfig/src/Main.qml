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

    palette.window:           cBase
    palette.windowText:       cText
    palette.base:             cCrust
    palette.alternateBase:    cMantle
    palette.text:             cText
    palette.button:           cCrust
    palette.buttonText:       cText
    palette.highlight:        cAccent
    palette.highlightedText:  cOnAccent
    palette.placeholderText:  cOverlay
    palette.mid:              cSurfaceVar
    palette.dark:             cMantle
    palette.light:            cCrust

    property int currentPage: 0
    property var pageModel: [
        {name:"General",      page:"pages/GeneralPage.qml"},
        {name:"Decoration",   page:"pages/DecorationPage.qml"},
        {name:"Animation",    page:"pages/AnimationPage.qml"},
        {name:"Tiling",       page:"pages/TilingPage.qml"},
        {name:"Panel",        page:"pages/PanelPage.qml"},
        {name:"Modules",      page:"pages/ModulesPage.qml"},
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
                        color: currentPage===index
                               ? Qt.rgba(root.cAccent.r,root.cAccent.g,root.cAccent.b,0.15)
                               : (_nma.containsMouse ? Qt.rgba(1,1,1,0.05) : "transparent")
                        Text {
                            anchors.verticalCenter:parent.verticalCenter
                            anchors.left:parent.left; anchors.leftMargin:14
                            text:modelData.name; font.pixelSize:13
                            color: currentPage===index ? root.cAccent : root.cText
                        }
                        MouseArea {
                            id:_nma; anchors.fill:parent
                            hoverEnabled:true; cursorShape:Qt.PointingHandCursor
                            onClicked:{ currentPage=index; pageLoader.source=modelData.page }
                        }
                    }
                }

                Item { Layout.fillHeight: true }

                Rectangle {
                    Layout.fillWidth:true; height:32; radius:6
                    color: configManager.connected
                           ? Qt.rgba(0,0.6,0.3,0.12)
                           : Qt.rgba(0.8,0,0,0.12)
                    Text {
                        anchors.centerIn:parent; font.pixelSize:11
                        text: configManager.connected ? "● Connected" : "○ Disconnected"
                        color: configManager.connected ? root.cSecondary : root.cError
                    }
                    MouseArea {
                        anchors.fill:parent; cursorShape:Qt.PointingHandCursor
                        onClicked: if(!configManager.connected) configManager.reconnect()
                    }
                }

                RowLayout { Layout.fillWidth:true; spacing:8
                    Rectangle {
                        Layout.fillWidth:true; height:38; radius:8
                        color:_sma.containsMouse?root.cCrust:"transparent"
                        border.color:root.cSurfaceVar; border.width:1
                        Text { anchors.centerIn:parent; text:"Save"; font.pixelSize:13; color:root.cText }
                        MouseArea { id:_sma; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor; onClicked:configManager.save() }
                    }
                    Rectangle {
                        Layout.fillWidth:true; height:38; radius:8
                        color:_ama.containsMouse?Qt.lighter(root.cAccent,1.1):root.cAccent
                        Text { anchors.centerIn:parent; text:"Apply"; font.pixelSize:13; font.bold:true; color:root.cOnAccent }
                        MouseArea { id:_ama; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor; onClicked:configManager.saveAndReload() }
                    }
                }
            }
        }

        // ---- content ----
        Rectangle {
            Layout.fillWidth:true; Layout.fillHeight:true; color:cBase
            Loader { id:pageLoader; anchors.fill:parent; source:"pages/GeneralPage.qml" }
        }
    }

    // ---- toast ----
    Rectangle {
        id: toast
        anchors.horizontalCenter:parent.horizontalCenter
        anchors.bottom:parent.bottom; anchors.bottomMargin:24
        width:_tt.width+32; height:40; radius:20; color:cSecondary; visible:false
        property alias text: _tt.text
        function show(m){ text=m; visible=true; _ttmr.restart() }
        Text { id:_tt; anchors.centerIn:parent; color:root.cOnAccent; font.pixelSize:12; font.bold:true }
        Timer { id:_ttmr; interval:2500; onTriggered:toast.visible=false }
    }

    Connections {
        target: configManager
        function onReloadSuccess()    { toast.show("Config reloaded") }
        function onReloadFailed(e)    { toast.show("Reload failed: "+e) }
        function onSaved()            { toast.show("Saved") }
    }
    Connections {
        target: panelConfigWriter
        function onSaved()  { toast.show("Panel config saved") }
        function onError(e) { toast.show(e) }
    }
}
