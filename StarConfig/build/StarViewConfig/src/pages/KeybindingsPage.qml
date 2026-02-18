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
