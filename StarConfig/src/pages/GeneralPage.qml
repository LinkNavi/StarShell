import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

ScrollView {
    id: root
    objectName: "general"
    
    ColumnLayout {
        width: root.width - 40
        spacing: 20
        
        Text {
            text: "General Settings"
            font.pixelSize: 28
            font.bold: true
            color: "#cdd6f4"
            Layout.topMargin: 30
            Layout.leftMargin: 30
        }
        
        SettingsGroup {
            title: "Window Gaps"
            Layout.fillWidth: true
            Layout.margins: 30
            
            SettingsRow {
                label: "Gap Size"
                description: "Space between windows"
                
                SpinBox {
                    from: 0
                    to: 100
                    value: configManager.gapSize
                    onValueModified: configManager.gapSize = value
                }
            }
        }
        
        SettingsGroup {
            title: "Window Borders"
            Layout.fillWidth: true
            Layout.margins: 30
            
            SettingsRow {
                label: "Border Width"
                description: "Window border thickness"
                
                SpinBox {
                    from: 0
                    to: 10
                    value: configManager.borderWidth
                    onValueModified: configManager.borderWidth = value
                }
            }
            
            SettingsRow {
                label: "Border Color"
                description: "Inactive window border"
                
                Rectangle {
                    width: 100
                    height: 40
                    radius: 6
                    color: configManager.borderColor
                    border.color: "#45475a"
                    border.width: 1
                    
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            // TODO: Color picker dialog
                        }
                    }
                }
            }
            
            SettingsRow {
                label: "Focused Border Color"
                description: "Active window border"
                
                Rectangle {
                    width: 100
                    height: 40
                    radius: 6
                    color: configManager.focusedBorderColor
                    border.color: "#45475a"
                    border.width: 1
                    
                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            // TODO: Color picker dialog
                        }
                    }
                }
            }
        }
        
        Item { Layout.fillHeight: true }
    }
}
