import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: panel

    visible: false
    width: 1920
    height: panelConfig.height
    color: "transparent"
    flags: Qt.FramelessWindowHint

    property int currentWorkspace: 1
    property string focusedWindowTitle: ""

    // Resolved colors
    readonly property color bgColor:     panelConfig.bgColor     || "#1e1e2e"
    readonly property color fgColor:     panelConfig.fgColor     || "#cdd6f4"
    readonly property color accentColor: panelConfig.workspaceActiveColor || panelConfig.accentColor || "#89b4fa"
    readonly property color dimColor:    Qt.rgba(fgColor.r, fgColor.g, fgColor.b, 0.5)

    // ── Background ───────────────────────────────────────────────────────────
    Rectangle {
        id: bgRect
        anchors.fill: parent
        anchors.margins: panelConfig.floating ? panelConfig.margins : 0
        radius:  panelConfig.floating ? panelConfig.cornerRadius : 0
        color:   panel.bgColor
        opacity: panelConfig.opacity
        clip:    true

        Rectangle {
            anchors.left: parent.left; anchors.right: parent.right; anchors.top: parent.top
            height: 1; color: "#20ffffff"
        }
    }

    // ── Content ──────────────────────────────────────────────────────────────
    RowLayout {
        anchors.fill: parent
        anchors.margins: panelConfig.floating ? (panelConfig.margins + 4) : 5
        spacing: 0

        // ══ LEFT ZONE ════════════════════════════════════════════════════════
        RowLayout {
            id: leftZone
            spacing: 6
            Layout.fillHeight: true

            // Built-in: workspace buttons
            Item {
                Layout.fillHeight: true
                Layout.preferredWidth: workspaceRow.width
                visible: panelConfig.showWorkspaces

                Row {
                    id: workspaceRow
                    anchors.centerIn: parent
                    spacing: 6

                    Repeater {
                        model: panelConfig.workspaceCount
                        WorkspaceButton {
                            number: index + 1
                            active: panel.currentWorkspace === (index + 1)
                            accentColor: panel.accentColor
                            fgColor:     panel.fgColor
                            bgColor:     panel.bgColor
                            inactiveColor: panelConfig.workspaceInactiveColor || "#94a3b8"
                            barHeight: panelConfig.height
                            onClicked: {
                                panel.currentWorkspace = number
                                ipcHandler.switchWorkspace(number)
                            }
                        }
                    }
                }
            }

            // Custom modules — left slot
            Repeater {
                model: moduleSlotModel("left")
                delegate: moduleDelegate
            }
        }

        // Separator left|center
        Rectangle {
            visible: panelConfig.showTitle || hasModulesInSlot("center")
            Layout.fillHeight: true
            Layout.topMargin: 8; Layout.bottomMargin: 8
            Layout.leftMargin: 6; Layout.rightMargin: 6
            width: 1; color: panel.dimColor; opacity: 0.3
        }

        // ══ CENTER ZONE ══════════════════════════════════════════════════════
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Built-in: window title
            Text {
                id: titleText
                anchors.centerIn: parent
                visible: panelConfig.showTitle && !hasModulesInSlot("center")
                width: Math.min(implicitWidth, parent.width - 20)
                text:  panel.focusedWindowTitle || "StarView"
                font.pixelSize: 13
                font.weight: panel.focusedWindowTitle ? Font.Normal : Font.Bold
                color: panel.focusedWindowTitle ? panel.fgColor : panel.accentColor
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
            }

            // Custom modules — center slot
            RowLayout {
                anchors.centerIn: parent
                spacing: 10
                visible: hasModulesInSlot("center")

                Repeater {
                    model: moduleSlotModel("center")
                    delegate: moduleDelegate
                }
            }
        }

        // Separator center|right
        Rectangle {
            visible: (panelConfig.showSystray || panelConfig.showClock || hasModulesInSlot("right"))
                     && (panelConfig.showTitle || hasModulesInSlot("center"))
            Layout.fillHeight: true
            Layout.topMargin: 8; Layout.bottomMargin: 8
            Layout.leftMargin: 6; Layout.rightMargin: 6
            width: 1; color: panel.dimColor; opacity: 0.3
        }

        // ══ RIGHT ZONE ═══════════════════════════════════════════════════════
        RowLayout {
            spacing: 10
            Layout.fillHeight: true

            // Custom modules — right slot
            Repeater {
                model: moduleSlotModel("right")
                delegate: moduleDelegate
            }

            // Built-in: system tray
            SystemTray {
                visible: panelConfig.showSystray
                Layout.fillHeight: true
                fgColor:     panel.fgColor
                dimColor:    panel.dimColor
                accentColor: panel.accentColor
            }

            // Separator before clock
            Rectangle {
                visible: panelConfig.showClock && panelConfig.showSystray
                Layout.fillHeight: true; Layout.topMargin: 8; Layout.bottomMargin: 8
                width: 1; color: panel.dimColor; opacity: 0.3
            }

            // Built-in: clock
            ClockWidget {
                visible: panelConfig.showClock
                Layout.fillHeight: true
                Layout.preferredWidth: implicitWidth
                fgColor:     panel.fgColor
                dimColor:    panel.dimColor
                clockFormat: panelConfig.clockFormat
                showDate:    panelConfig.showDate
            }
        }
    }

    // ── Module helpers ───────────────────────────────────────────────────────

    // Returns a filtered+sorted JS array of module objects for a given slot
    function moduleSlotModel(slot) {
        var result = []
        var count  = moduleServer.model.rowCount()
        for (var i = 0; i < count; i++) {
            var idx  = moduleServer.model.index(i, 0)
            var s    = moduleServer.model.data(idx, 258 /*SlotRole*/)
            if (s === slot) {
                result.push({
                    name:      moduleServer.model.data(idx, 257 /*NameRole*/),
                    text:      moduleServer.model.data(idx, 259 /*TextRole*/),
                    icon:      moduleServer.model.data(idx, 260 /*IconRole*/),
                    tooltip:   moduleServer.model.data(idx, 261 /*TooltipRole*/),
                    color:     moduleServer.model.data(idx, 262 /*ColorRole*/),
                    type:      moduleServer.model.data(idx, 263 /*TypeRole*/),
                    qmlPath:   moduleServer.model.data(idx, 264 /*QmlPathRole*/),
                    slot:      s,
                    order:     moduleServer.model.data(idx, 265 /*OrderRole*/),
                    connected: moduleServer.model.data(idx, 266 /*ConnectedRole*/),
                })
            }
        }
        result.sort(function(a, b) { return a.order - b.order })
        return result
    }

    function hasModulesInSlot(slot) {
        return moduleSlotModel(slot).length > 0
    }

    // Delegate used by all three Repeaters
    Component {
        id: moduleDelegate
        ModuleSlot {
            Layout.fillHeight: true
            moduleData:  modelData
            fgColor:     panel.fgColor
            dimColor:    panel.dimColor
            accentColor: panel.accentColor
        }
    }

    // Re-evaluate slot models when module data changes
    Connections {
        target: moduleServer
        function onModuleUpdated()  { panel.update() }
        function onSlotsChanged()   { panel.update() }
    }

    // ── IPC connections ──────────────────────────────────────────────────────
    Connections {
        target: ipcHandler
        function onWorkspaceChanged(ws)      { panel.currentWorkspace    = ws }
        function onFocusedWindowChanged(title) { panel.focusedWindowTitle = title }
    }

    Component.onCompleted: {
        ipcHandler.connectToCompositor()

        // Apply slot ordering from panel config if available
        if (typeof panelConfig.modulesLeft !== "undefined") {
            moduleServer.applySlots(
                panelConfig.modulesLeft,
                panelConfig.modulesCenter,
                panelConfig.modulesRight
            )
        }
    }
}
