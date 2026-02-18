import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root

    property var moduleData: null
    property color fgColor:     "#cdd6f4"
    property color dimColor:    "#6c7086"
    property color accentColor: "#89b4fa"

    implicitWidth:  _inner.implicitWidth + 10
    implicitHeight: parent ? parent.height : 30

    // ── QML module ──────────────────────────────────────────────────────────
    Loader {
        id: qmlLoader
        anchors.fill: parent
        active: moduleData && moduleData.type === "qml" && moduleData.qmlPath !== ""
        source: active ? ("file://" + moduleData.qmlPath) : ""

        onStatusChanged: {
            if (status === Loader.Error)
                console.warn("ModuleSlot: failed to load", moduleData ? moduleData.qmlPath : "")
        }

        onLoaded: {
            if (item) {
                if ("fgColor"     in item) item.fgColor     = Qt.binding(function() { return root.fgColor })
                if ("dimColor"    in item) item.dimColor    = Qt.binding(function() { return root.dimColor })
                if ("accentColor" in item) item.accentColor = Qt.binding(function() { return root.accentColor })
            }
        }
    }

    // ── IPC / script text widget ─────────────────────────────────────────────
    Item {
        id: _inner
        anchors.centerIn: parent
        visible: !qmlLoader.active
        implicitWidth:  _row.implicitWidth
        implicitHeight: _row.implicitHeight

        RowLayout {
            id: _row
            anchors.centerIn: parent
            spacing: 5

            Text {
                id: _icon
                visible: text !== ""
                text: moduleData ? (moduleData.icon || "") : ""
                font.pixelSize: 14
                font.family: "Nerd Font,monospace"
                color: (moduleData && moduleData.color !== "") ? moduleData.color : root.fgColor
            }

            Text {
                id: _label
                visible: text !== ""
                text: moduleData ? (moduleData.text || "") : ""
                font.pixelSize: 12
                color: (moduleData && moduleData.color !== "") ? moduleData.color : root.fgColor
            }

            // Disconnected dot for IPC modules with no text yet
            Text {
                visible: moduleData
                         && moduleData.type === "ipc"
                         && !moduleData.connected
                         && moduleData.text === ""
                text: "○"
                font.pixelSize: 10
                color: root.dimColor
            }
        }
    }

    // ── Tooltip ──────────────────────────────────────────────────────────────
    ToolTip {
        id: _tooltip
        visible:  _ma.containsMouse && moduleData !== null && moduleData.tooltip !== ""
        text:     moduleData ? (moduleData.tooltip || "") : ""
        delay:    600
    }

    MouseArea {
        id: _ma
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.NoButton
    }
}
