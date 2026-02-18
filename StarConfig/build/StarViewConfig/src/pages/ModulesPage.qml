import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    id: modPage

    readonly property string modulesDir: Qt.resolvedUrl(
        "file://" + StandardPaths.writableLocation(StandardPaths.HomeLocation)
        + "/.config/starview/panel/modules/")

    // Local mirror of panel.json module slot arrays
    property var modulesLeft:   []
    property var modulesCenter: []
    property var modulesRight:  []

    // All discovered module names (from the directory)
    property var discoveredModules: []

    Component.onCompleted: loadFromPanelJson()

    // ── Helpers ──────────────────────────────────────────────────────────────

    function loadFromPanelJson() {
        try {
            var raw = panelConfigWriter.load()
            if (raw && raw !== "{}") {
                var cfg = JSON.parse(raw)
                modulesLeft   = cfg.modules_left   || []
                modulesCenter = cfg.modules_center  || []
                modulesRight  = cfg.modules_right   || []
            }
        } catch(e) {
            console.log("ModulesPage: could not read panel.json:", e)
        }
        discoverModules()
    }

    function discoverModules() {
        // Ask C++ side what it found in the modules directory
        // moduleServer.model exposes every scanned module
        var found = []
        var count = moduleServer.model.rowCount()
        for (var i = 0; i < count; i++) {
            var idx  = moduleServer.model.index(i, 0)
            var name = moduleServer.model.data(idx, 257 /*NameRole*/)
            found.push(name)
        }
        discoveredModules = found
    }

    function isAssigned(name) {
        return modulesLeft.indexOf(name)   >= 0
            || modulesCenter.indexOf(name) >= 0
            || modulesRight.indexOf(name)  >= 0
    }

    function slotOf(name) {
        if (modulesLeft.indexOf(name)   >= 0) return "left"
        if (modulesCenter.indexOf(name) >= 0) return "center"
        if (modulesRight.indexOf(name)  >= 0) return "right"
        return ""
    }

    function assignToSlot(name, slot) {
        removeFromAllSlots(name)
        if (slot === "left")   { var l = modulesLeft.slice();   l.push(name); modulesLeft   = l }
        if (slot === "center") { var c = modulesCenter.slice(); c.push(name); modulesCenter = c }
        if (slot === "right")  { var r = modulesRight.slice();  r.push(name); modulesRight  = r }
    }

    function removeFromAllSlots(name) {
        modulesLeft   = modulesLeft.filter(  function(n){ return n !== name })
        modulesCenter = modulesCenter.filter(function(n){ return n !== name })
        modulesRight  = modulesRight.filter( function(n){ return n !== name })
    }

    function moveUp(slot, idx) {
        if (idx === 0) return
        var arr = slotArray(slot).slice()
        var tmp = arr[idx - 1]; arr[idx - 1] = arr[idx]; arr[idx] = tmp
        setSlotArray(slot, arr)
    }

    function moveDown(slot, idx) {
        var arr = slotArray(slot).slice()
        if (idx >= arr.length - 1) return
        var tmp = arr[idx + 1]; arr[idx + 1] = arr[idx]; arr[idx] = tmp
        setSlotArray(slot, arr)
    }

    function slotArray(slot) {
        if (slot === "left")   return modulesLeft
        if (slot === "center") return modulesCenter
        return modulesRight
    }

    function setSlotArray(slot, arr) {
        if (slot === "left")   modulesLeft   = arr
        if (slot === "center") modulesCenter = arr
        if (slot === "right")  modulesRight  = arr
    }

    function save() {
        try {
            var raw = panelConfigWriter.load()
            var cfg = (raw && raw !== "{}") ? JSON.parse(raw) : {}
            cfg.modules_left   = modulesLeft
            cfg.modules_center = modulesCenter
            cfg.modules_right  = modulesRight
            panelConfigWriter.save(JSON.stringify(cfg, null, 2))

            // Tell the live panel to re-apply slots
            moduleServer.applySlots(modulesLeft, modulesCenter, modulesRight)
        } catch(e) {
            console.warn("ModulesPage: save failed:", e)
        }
    }

    function createExampleModule(type) {
        // Creates a scaffold in ~/.config/starview/panel/modules/<name>/
        // We do it by writing the file through PanelConfigWriter's path logic
        // (actual folder creation is done in C++ on next scan)
        var name = _newNameField.text.trim()
        if (!name) return

        var manifest = {
            name:  name,
            type:  type,
            slot:  "right",
            order: 99,
            icon:  "",
            default_text: ""
        }

        if (type === "script") {
            manifest.exec     = "run.sh"
            manifest.interval = 5000
        }

        // Write manifest via a dedicated invokable
        moduleServer.createModule(name, JSON.stringify(manifest, null, 2), type)
        _newNameField.text = ""
        discoverModules()
    }

    // Re-discover when server rescans
    Connections {
        target: moduleServer
        function onSlotsChanged() { modPage.discoverModules() }
    }

    // ── UI ───────────────────────────────────────────────────────────────────
    ScrollView {
        anchors.fill: parent
        contentWidth: availableWidth

        ColumnLayout {
            width: parent.width - 60
            x: 30
            spacing: 20

            Text {
                text: "Modules"
                font.pixelSize: 26; font.bold: true
                color: root.cText
                Layout.topMargin: 30
            }
            Text {
                text: "Modules live in ~/.config/starview/panel/modules/<name>/"
                font.pixelSize: 12; color: root.cSubtext
                Layout.bottomMargin: 4
            }

            // ── Create new module scaffold ───────────────────────────────────
            SettingsGroup {
                title: "Create New Module"

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    TextField {
                        id: _newNameField
                        Layout.fillWidth: true
                        placeholderText: "module-name (lowercase, no spaces)"
                        font.pixelSize: 12; color: root.cText
                        background: Rectangle {
                            color: root.cCrust; radius: 6
                            border.color: root.cSurfaceVar; border.width: 1
                        }
                    }

                    Rectangle {
                        width: 90; height: 36; radius: 6
                        color: _qmlMa.containsMouse
                               ? Qt.lighter(root.cAccent, 1.1) : root.cAccent
                        Text {
                            anchors.centerIn: parent
                            text: "QML"
                            font.pixelSize: 13; font.bold: true
                            color: root.cOnAccent
                        }
                        MouseArea {
                            id: _qmlMa; anchors.fill: parent
                            hoverEnabled: true; cursorShape: Qt.PointingHandCursor
                            onClicked: modPage.createExampleModule("qml")
                        }
                    }

                    Rectangle {
                        width: 90; height: 36; radius: 6
                        color: _scriptMa.containsMouse
                               ? Qt.lighter(root.cSecondary, 1.1) : root.cSecondary
                        Text {
                            anchors.centerIn: parent
                            text: "Script"
                            font.pixelSize: 13; font.bold: true
                            color: root.cOnAccent
                        }
                        MouseArea {
                            id: _scriptMa; anchors.fill: parent
                            hoverEnabled: true; cursorShape: Qt.PointingHandCursor
                            onClicked: modPage.createExampleModule("script")
                        }
                    }

                    Rectangle {
                        width: 90; height: 36; radius: 6
                        color: _ipcMa.containsMouse
                               ? Qt.lighter(root.cTertiary, 1.1) : root.cTertiary
                        Text {
                            anchors.centerIn: parent
                            text: "IPC"
                            font.pixelSize: 13; font.bold: true
                            color: root.cOnAccent
                        }
                        MouseArea {
                            id: _ipcMa; anchors.fill: parent
                            hoverEnabled: true; cursorShape: Qt.PointingHandCursor
                            onClicked: modPage.createExampleModule("ipc")
                        }
                    }
                }

                // Mini cheat-sheet
                Rectangle {
                    Layout.fillWidth: true
                    color: root.cCrust; radius: 8
                    implicitHeight: _cheat.implicitHeight + 20

                    Text {
                        id: _cheat
                        anchors { left: parent.left; right: parent.right; top: parent.top; margins: 12 }
                        font.pixelSize: 11; font.family: "monospace"
                        color: root.cSubtext; wrapMode: Text.WrapAnywhere
                        text:
"IPC module  — connect to /tmp/starview-modules.sock, send newline-delimited JSON:\n" +
"  {\"module\":\"my-mod\", \"text\":\"42%\", \"icon\":\"󰻠\", \"tooltip\":\"CPU\", \"color\":\"#f38ba8\"}\n\n" +
"Script module — run.sh stdout: plain text  OR  JSON object same as above\n\n" +
"QML module  — index.qml receives fgColor / dimColor / accentColor properties"
                    }
                }
            }

            // ── Slot assignment ──────────────────────────────────────────────
            SettingsGroup {
                title: "Discovered Modules (" + modPage.discoveredModules.length + ")"

                Text {
                    visible: modPage.discoveredModules.length === 0
                    text: "No modules found. Create one above or drop a folder into\n~/.config/starview/panel/modules/"
                    font.pixelSize: 12; color: root.cSubtext
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                }

                Repeater {
                    model: modPage.discoveredModules

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        // Module name + type badge
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 2
                            Text {
                                text: modelData
                                font.pixelSize: 13; color: root.cText
                            }
                            Text {
                                text: modPage.slotOf(modelData) !== ""
                                      ? "→ " + modPage.slotOf(modelData)
                                      : "unassigned"
                                font.pixelSize: 10
                                color: modPage.isAssigned(modelData) ? root.cAccent : root.cSubtext
                            }
                        }

                        // Slot selector
                        StyledComboBox {
                            model: ["(none)", "left", "center", "right"]
                            currentIndex: {
                                var s = modPage.slotOf(modelData)
                                if (s === "left")   return 1
                                if (s === "center") return 2
                                if (s === "right")  return 3
                                return 0
                            }
                            implicitWidth: 100
                            onActivated: {
                                var slot = currentText
                                if (slot === "(none)") modPage.removeFromAllSlots(modelData)
                                else modPage.assignToSlot(modelData, slot)
                            }
                            background: Rectangle {
                                color: root.cCrust; radius: 6
                                border.color: root.cSurfaceVar; border.width: 1
                            }
                        }
                    }
                }
            }

            // ── Per-slot ordering ────────────────────────────────────────────
            Repeater {
                model: ["left", "center", "right"]

                SettingsGroup {
                    title: modelData.charAt(0).toUpperCase() + modelData.slice(1)
                           + " Slot (" + modPage.slotArray(modelData).length + ")"
                    visible: modPage.slotArray(modelData).length > 0

                    Repeater {
                        model: modPage.slotArray(modelData)

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 8

                            Text {
                                text: modelData
                                font.pixelSize: 13; color: root.cText
                                Layout.fillWidth: true
                            }

                            // Up
                            Rectangle {
                                width: 28; height: 28; radius: 6
                                color: _upMa.containsMouse ? root.cCrust : "transparent"
                                border.color: root.cSurfaceVar; border.width: 1
                                Text { anchors.centerIn: parent; text: "↑"; color: root.cText; font.pixelSize: 13 }
                                MouseArea {
                                    id: _upMa; anchors.fill: parent
                                    hoverEnabled: true; cursorShape: Qt.PointingHandCursor
                                    onClicked: modPage.moveUp(modelData, index)  // modelData here = slot name from outer Repeater
                                }
                            }

                            // Down
                            Rectangle {
                                width: 28; height: 28; radius: 6
                                color: _dnMa.containsMouse ? root.cCrust : "transparent"
                                border.color: root.cSurfaceVar; border.width: 1
                                Text { anchors.centerIn: parent; text: "↓"; color: root.cText; font.pixelSize: 13 }
                                MouseArea {
                                    id: _dnMa; anchors.fill: parent
                                    hoverEnabled: true; cursorShape: Qt.PointingHandCursor
                                    onClicked: modPage.moveDown(modelData, index)
                                }
                            }

                            // Remove
                            Text {
                                text: "✕"; color: root.cError; font.pixelSize: 16
                                MouseArea {
                                    anchors.fill: parent; anchors.margins: -4
                                    cursorShape: Qt.PointingHandCursor
                                    onClicked: modPage.removeFromAllSlots(modelData)
                                }
                            }
                        }
                    }
                }
            }

            // ── Save ─────────────────────────────────────────────────────────
            Rectangle {
                Layout.fillWidth: true; height: 44; radius: 8
                color: _saveMa.containsMouse ? Qt.lighter(root.cAccent, 1.1) : root.cAccent

                Text {
                    anchors.centerIn: parent
                    text: "Save Module Config"
                    font.pixelSize: 14; font.bold: true; color: root.cOnAccent
                }
                MouseArea {
                    id: _saveMa; anchors.fill: parent
                    hoverEnabled: true; cursorShape: Qt.PointingHandCursor
                    onClicked: modPage.save()
                }
            }

            Item { height: 30 }
        }
    }
}
