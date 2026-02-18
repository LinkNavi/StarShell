import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    id: panelPg

    // Local state
    property int panelHeight: 40
    property int panelPosition: 0
    property bool showWorkspaces: true
    property bool showTitle: true
    property bool showClock: true
    property bool showSystray: true
    property int workspaceCount: 9
    property string clockFormat: "hh:mm"
    property bool showDate: false
    property real panelOpacity: 0.95
    property int cornerRadius: 0
    property int panelMargins: 0
    property bool floating: false
    property string bgColor: ""
    property string fgColor: ""
    property string accentColor: ""
    property string wsActiveColor: ""
    property string wsInactiveColor: ""

    readonly property var clockFormats: ["hh:mm", "hh:mm:ss", "h:mm AP", "h:mm:ss AP", "HH:mm"]

    Component.onCompleted: {
        try {
            var json = panelConfigWriter.load()
            if (json && json !== "{}") {
                var cfg = JSON.parse(json)
                if (cfg.height !== undefined) panelHeight = cfg.height
                if (cfg.position !== undefined) panelPosition = cfg.position
                if (cfg.show_workspaces !== undefined) showWorkspaces = cfg.show_workspaces
                if (cfg.show_title !== undefined) showTitle = cfg.show_title
                if (cfg.show_clock !== undefined) showClock = cfg.show_clock
                if (cfg.show_systray !== undefined) showSystray = cfg.show_systray
                if (cfg.workspace_count !== undefined) workspaceCount = cfg.workspace_count
                if (cfg.clock_format !== undefined) clockFormat = cfg.clock_format
                if (cfg.show_date !== undefined) showDate = cfg.show_date
                if (cfg.opacity !== undefined) panelOpacity = cfg.opacity
                if (cfg.corner_radius !== undefined) cornerRadius = cfg.corner_radius
                if (cfg.margins !== undefined) panelMargins = cfg.margins
                if (cfg.floating !== undefined) floating = cfg.floating
                if (cfg.bg_color !== undefined) bgColor = cfg.bg_color
                if (cfg.fg_color !== undefined) fgColor = cfg.fg_color
                if (cfg.accent_color !== undefined) accentColor = cfg.accent_color
                if (cfg.workspace_active_color !== undefined) wsActiveColor = cfg.workspace_active_color
                if (cfg.workspace_inactive_color !== undefined) wsInactiveColor = cfg.workspace_inactive_color
            }
        } catch(e) {
            console.log("No existing panel config, using defaults")
        }
    }

    function buildJson() {
        var cfg = {}
        cfg.height = panelHeight
        cfg.position = panelPosition
        cfg.show_workspaces = showWorkspaces
        cfg.show_title = showTitle
        cfg.show_clock = showClock
        cfg.show_systray = showSystray
        cfg.workspace_count = workspaceCount
        cfg.clock_format = clockFormat
        cfg.show_date = showDate
        cfg.opacity = panelOpacity
        cfg.corner_radius = cornerRadius
        cfg.margins = panelMargins
        cfg.floating = floating
        if (bgColor !== "") cfg.bg_color = bgColor
        if (fgColor !== "") cfg.fg_color = fgColor
        if (accentColor !== "") cfg.accent_color = accentColor
        if (wsActiveColor !== "") cfg.workspace_active_color = wsActiveColor
        if (wsInactiveColor !== "") cfg.workspace_inactive_color = wsInactiveColor
        return JSON.stringify(cfg, null, 2)
    }

    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth

        ColumnLayout {
            width: parent.width - 60
            x: 30
            spacing: 20

            Text { text: "Panel (StarBar)"; font.pixelSize: 26; font.bold: true; color: root.cText; Layout.topMargin: 30 }
            Text { text: "Configure the StarView panel bar. Saves to ~/.config/starview/panel.json"
                   font.pixelSize: 12; color: root.cSubtext; Layout.bottomMargin: 8 }

            // ---- Layout ----
            SettingsGroup {
                title: "Layout"

                SettingsRow {
                    label: "Height (px)"
                    SpinBox {
                        from: 24; to: 64; value: panelPg.panelHeight
                        onValueChanged: panelPg.panelHeight = value
                        background: Rectangle { color: root.cCrust; radius: 6; border.color: root.cSurfaceVar; border.width: 1 }
                    }
                }

                SettingsRow {
                    label: "Position"
                    StyledComboBox {
                        model: ["Top", "Bottom"]
                        currentIndex: panelPg.panelPosition
                        onActivated: panelPg.panelPosition = currentIndex
                        background: Rectangle { color: root.cCrust; radius: 6; border.color: root.cSurfaceVar; border.width: 1 }
                    }
                }

                SettingsRow {
                    label: "Floating"
                    Switch { checked: panelPg.floating; onCheckedChanged: panelPg.floating = checked }
                }

                SettingsRow {
                    label: "Corner Radius"
                    desc: "Only applies when floating"
                    SpinBox {
                        from: 0; to: 24; value: panelPg.cornerRadius; enabled: panelPg.floating
                        onValueChanged: panelPg.cornerRadius = value
                        background: Rectangle { color: root.cCrust; radius: 6; border.color: root.cSurfaceVar; border.width: 1 }
                    }
                }

                SettingsRow {
                    label: "Margins"
                    desc: "Gap from screen edges (floating)"
                    SpinBox {
                        from: 0; to: 20; value: panelPg.panelMargins; enabled: panelPg.floating
                        onValueChanged: panelPg.panelMargins = value
                        background: Rectangle { color: root.cCrust; radius: 6; border.color: root.cSurfaceVar; border.width: 1 }
                    }
                }

                SettingsRow {
                    label: "Opacity"
                    RowLayout {
                        spacing: 8
                        Slider {
                            from: 0.3; to: 1.0; stepSize: 0.05; value: panelPg.panelOpacity; implicitWidth: 160
                            onMoved: panelPg.panelOpacity = value
                        }
                        Text { text: panelPg.panelOpacity.toFixed(2); font.pixelSize: 12; color: root.cSubtext }
                    }
                }
            }

            // ---- Modules ----
            SettingsGroup {
                title: "Modules"

                SettingsRow {
                    label: "Show Workspaces"
                    Switch { checked: panelPg.showWorkspaces; onCheckedChanged: panelPg.showWorkspaces = checked }
                }

                SettingsRow {
                    label: "Workspace Count"
                    desc: "Number of workspace buttons"
                    SpinBox {
                        from: 1; to: 20; value: panelPg.workspaceCount; enabled: panelPg.showWorkspaces
                        onValueChanged: panelPg.workspaceCount = value
                        background: Rectangle { color: root.cCrust; radius: 6; border.color: root.cSurfaceVar; border.width: 1 }
                    }
                }

                SettingsRow {
                    label: "Show Window Title"
                    Switch { checked: panelPg.showTitle; onCheckedChanged: panelPg.showTitle = checked }
                }

                SettingsRow {
                    label: "Show System Tray"
                    Switch { checked: panelPg.showSystray; onCheckedChanged: panelPg.showSystray = checked }
                }

                SettingsRow {
                    label: "Show Clock"
                    Switch { checked: panelPg.showClock; onCheckedChanged: panelPg.showClock = checked }
                }

                SettingsRow {
                    label: "Clock Format"
                    StyledComboBox {
                        model: panelPg.clockFormats
                        currentIndex: Math.max(0, panelPg.clockFormats.indexOf(panelPg.clockFormat))
                        enabled: panelPg.showClock
                        onActivated: panelPg.clockFormat = currentText
                        background: Rectangle { color: root.cCrust; radius: 6; border.color: root.cSurfaceVar; border.width: 1 }
                    }
                }

                SettingsRow {
                    label: "Show Date"
                    Switch { checked: panelPg.showDate; enabled: panelPg.showClock; onCheckedChanged: panelPg.showDate = checked }
                }
            }

            // ---- Colors ----
            SettingsGroup {
                title: "Colors"

                Text { text: "Leave empty to use matugen/default colors"; font.pixelSize: 11; color: root.cSubtext }

                SettingsRow {
                    label: "Background"
                    TextField {
                        text: panelPg.bgColor; implicitWidth: 120; font.pixelSize: 12; color: root.cText
                        placeholderText: "#1e1e2e"
                        background: Rectangle { color: root.cCrust; radius: 6; border.color: root.cSurfaceVar; border.width: 1 }
                        onEditingFinished: panelPg.bgColor = text
                    }
                }

                SettingsRow {
                    label: "Foreground"
                    TextField {
                        text: panelPg.fgColor; implicitWidth: 120; font.pixelSize: 12; color: root.cText
                        placeholderText: "#cdd6f4"
                        background: Rectangle { color: root.cCrust; radius: 6; border.color: root.cSurfaceVar; border.width: 1 }
                        onEditingFinished: panelPg.fgColor = text
                    }
                }

                SettingsRow {
                    label: "Accent"
                    TextField {
                        text: panelPg.accentColor; implicitWidth: 120; font.pixelSize: 12; color: root.cText
                        placeholderText: "#89b4fa"
                        background: Rectangle { color: root.cCrust; radius: 6; border.color: root.cSurfaceVar; border.width: 1 }
                        onEditingFinished: panelPg.accentColor = text
                    }
                }

                SettingsRow {
                    label: "Workspace Active"
                    TextField {
                        text: panelPg.wsActiveColor; implicitWidth: 120; font.pixelSize: 12; color: root.cText
                        placeholderText: "#89b4fa"
                        background: Rectangle { color: root.cCrust; radius: 6; border.color: root.cSurfaceVar; border.width: 1 }
                        onEditingFinished: panelPg.wsActiveColor = text
                    }
                }

                SettingsRow {
                    label: "Workspace Inactive"
                    TextField {
                        text: panelPg.wsInactiveColor; implicitWidth: 120; font.pixelSize: 12; color: root.cText
                        placeholderText: "#94a3b8"
                        background: Rectangle { color: root.cCrust; radius: 6; border.color: root.cSurfaceVar; border.width: 1 }
                        onEditingFinished: panelPg.wsInactiveColor = text
                    }
                }

                Rectangle {
                    Layout.fillWidth: true; height: 36; radius: 6
                    color: colors.loaded ? (_pmma.containsMouse ? Qt.lighter(root.cTertiary, 1.1) : root.cTertiary) : root.cCrust
                    Text {
                        anchors.centerIn: parent; text: "Use Matugen Colors"; font.pixelSize: 12; font.bold: true
                        color: colors.loaded ? root.cOnAccent : root.cSubtext
                    }
                    MouseArea {
                        id: _pmma; anchors.fill: parent; hoverEnabled: true; cursorShape: Qt.PointingHandCursor
                        enabled: colors.loaded
                        onClicked: {
                            panelPg.bgColor = colors.background
                            panelPg.fgColor = colors.onBackground
                            panelPg.accentColor = colors.primary
                            panelPg.wsActiveColor = colors.primary
                            panelPg.wsInactiveColor = colors.outline
                        }
                    }
                }
            }

            // ---- Preview ----
            SettingsGroup {
                title: "Preview"

                Rectangle {
                    Layout.fillWidth: true; Layout.preferredHeight: 80; color: root.cCrust; radius: 8; clip: true

                    Rectangle {
                        anchors.horizontalCenter: parent.horizontalCenter
                        y: panelPg.panelPosition === 0 ? 10 : parent.height - height - 10
                        width: parent.width - 40
                        height: Math.max(panelPg.panelHeight * 0.6, 20)
                        radius: panelPg.floating ? panelPg.cornerRadius * 0.5 : 0
                        color: panelPg.bgColor || root.cMantle
                        opacity: panelPg.panelOpacity
                        border.color: panelPg.floating ? Qt.rgba(1, 1, 1, 0.1) : "transparent"
                        border.width: panelPg.floating ? 1 : 0

                        RowLayout {
                            anchors.fill: parent; anchors.margins: 4; spacing: 6

                            Row {
                                spacing: 3; visible: panelPg.showWorkspaces
                                Repeater {
                                    model: Math.min(panelPg.workspaceCount, 9)
                                    Rectangle {
                                        width: 8; height: 8; radius: 2
                                        color: index === 0 ? (panelPg.wsActiveColor || root.cAccent) : Qt.rgba(1, 1, 1, 0.15)
                                    }
                                }
                            }

                            Text {
                                Layout.fillWidth: true; visible: panelPg.showTitle
                                text: "Preview Window"; font.pixelSize: 8
                                color: panelPg.fgColor || root.cText
                                horizontalAlignment: Text.AlignHCenter; elide: Text.ElideRight
                            }

                            Text {
                                visible: panelPg.showClock; text: "12:00"; font.pixelSize: 8
                                color: panelPg.fgColor || root.cText
                            }
                        }
                    }
                }
            }

            // ---- Save ----
            Rectangle {
                Layout.fillWidth: true; height: 44; radius: 8
                color: _saveMa.containsMouse ? Qt.lighter(root.cAccent, 1.1) : root.cAccent

                Text {
                    anchors.centerIn: parent; text: "Save Panel Config"
                    font.pixelSize: 14; font.bold: true; color: root.cOnAccent
                }

                MouseArea {
                    id: _saveMa; anchors.fill: parent; hoverEnabled: true; cursorShape: Qt.PointingHandCursor
                    onClicked: panelConfigWriter.save(panelPg.buildJson())
                }
            }

            Item { height: 30 }
        }
    }
}
