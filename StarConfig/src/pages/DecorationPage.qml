import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

ScrollView {
    objectName: "decoration"
    
    ColumnLayout {
        width: parent.width - 40
        spacing: 20
        
        Text {
            text: "Decoration Settings"
            font.pixelSize: 28
            font.bold: true
            color: "#cdd6f4"
            Layout.topMargin: 30
            Layout.leftMargin: 30
        }
        
        SettingsGroup {
            title: "Titlebar"
            Layout.fillWidth: true
            Layout.margins: 30
            
            SettingsRow {
                label: "Enable Decorations"
                description: "Show window titlebars"
                
                Switch {
                    checked: configManager.decorationsEnabled
                    onToggled: configManager.decorationsEnabled = checked
                }
            }
            
            SettingsRow {
                label: "Titlebar Height"
                description: "Height of window titlebars"
                
                SpinBox {
                    from: 20
                    to: 50
                    value: configManager.titlebarHeight
                    onValueModified: configManager.titlebarHeight = value
                }
            }
        }
        
        Item { Layout.fillHeight: true }
    }
}
