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
