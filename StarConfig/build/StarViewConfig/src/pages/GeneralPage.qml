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
