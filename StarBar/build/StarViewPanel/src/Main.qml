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

    // Resolved colors: config overrides, then fallbacks
    readonly property color bgColor: panelConfig.bgColor || "#1e1e2e"
    readonly property color fgColor: panelConfig.fgColor || "#cdd6f4"
    readonly property color accentColor: panelConfig.workspaceActiveColor || panelConfig.accentColor || "#89b4fa"
    readonly property color dimColor: Qt.rgba(fgColor.r, fgColor.g, fgColor.b, 0.5)

    // Background
    Rectangle {
        id: bgRect
        anchors.fill: parent
        anchors.margins: panelConfig.floating ? panelConfig.margins : 0
        radius: panelConfig.floating ? panelConfig.cornerRadius : 0
        color: panel.bgColor
        opacity: panelConfig.opacity
        clip: true

        // Subtle top highlight
        Rectangle {
            anchors.left: parent.left; anchors.right: parent.right; anchors.top: parent.top
            height: 1; color: "#20ffffff"
        }
    }

    // Content
    RowLayout {
        anchors.fill: parent
        anchors.margins: panelConfig.floating ? (panelConfig.margins + 4) : 5
        spacing: 12

        // LEFT: Workspace buttons
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
                        fgColor: panel.fgColor
                        bgColor: panel.bgColor
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

        // Separator
        Rectangle {
            visible: panelConfig.showWorkspaces && panelConfig.showTitle
            Layout.fillHeight: true; Layout.topMargin: 8; Layout.bottomMargin: 8
            width: 1; color: panel.dimColor; opacity: 0.3
        }

        // CENTER: Title
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: panelConfig.showTitle

            Text {
                id: titleText
                anchors.centerIn: parent
                width: Math.min(implicitWidth, parent.width - 20)
                text: panel.focusedWindowTitle || "StarView"
                font.pixelSize: 13
                font.weight: panel.focusedWindowTitle ? Font.Normal : Font.Bold
                color: panel.focusedWindowTitle ? panel.fgColor : panel.accentColor
                elide: Text.ElideRight
                horizontalAlignment: Text.AlignHCenter
            }
        }

        // Separator
        Rectangle {
            visible: panelConfig.showSystray && (panelConfig.showTitle || panelConfig.showWorkspaces)
            Layout.fillHeight: true; Layout.topMargin: 8; Layout.bottomMargin: 8
            width: 1; color: panel.dimColor; opacity: 0.3
        }

        // RIGHT: System tray area
        SystemTray {
            visible: panelConfig.showSystray
            Layout.fillHeight: true
            fgColor: panel.fgColor
            dimColor: panel.dimColor
            accentColor: panel.accentColor
        }

        // Separator
        Rectangle {
            visible: panelConfig.showClock && panelConfig.showSystray
            Layout.fillHeight: true; Layout.topMargin: 8; Layout.bottomMargin: 8
            width: 1; color: panel.dimColor; opacity: 0.3
        }

        // RIGHT: Clock
        ClockWidget {
            visible: panelConfig.showClock
            Layout.fillHeight: true
            Layout.preferredWidth: implicitWidth
            fgColor: panel.fgColor
            dimColor: panel.dimColor
            clockFormat: panelConfig.clockFormat
            showDate: panelConfig.showDate
        }
    }

    // IPC connections
    Connections {
        target: ipcHandler
        function onWorkspaceChanged(ws) { panel.currentWorkspace = ws }
        function onFocusedWindowChanged(title) { panel.focusedWindowTitle = title }
    }

    Component.onCompleted: {
        ipcHandler.connectToCompositor()
    }
}
