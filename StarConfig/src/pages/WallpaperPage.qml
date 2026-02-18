import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "../components"

ScrollView {
    id: root
    objectName: "wallpaper"
    
    ColumnLayout {
        width: root.width - 40
        spacing: 20
        
        Text {
            text: "Wallpaper & Colors"
            font.pixelSize: 28
            font.bold: true
            color: "#cdd6f4"
            Layout.topMargin: 30
            Layout.leftMargin: 30
        }
        
        SettingsGroup {
            title: "Wallpaper"
            Layout.fillWidth: true
            Layout.margins: 30
            
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 15
                
                // Preview
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 300
                    radius: 12
                    color: "#11111b"
                    border.color: "#45475a"
                    border.width: 1
                    
                    Image {
                        anchors.fill: parent
                        anchors.margins: 2
                        source: configManager.wallpaperPath ? "file://" + configManager.wallpaperPath : ""
                        fillMode: Image.PreserveAspectFit
                        
                        Text {
                            anchors.centerIn: parent
                            text: "No wallpaper set"
                            color: "#6c7086"
                            visible: !configManager.wallpaperPath
                        }
                    }
                }
                
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    
                    Button {
                        text: "Select Wallpaper"
                        Layout.fillWidth: true
                        
                        background: Rectangle {
                            color: parent.pressed ? "#313244" : (parent.hovered ? "#45475a" : "#1e1e2e")
                            radius: 8
                            border.color: "#45475a"
                            border.width: 1
                        }
                        
                        contentItem: Text {
                            text: parent.text
                            color: "#cdd6f4"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        
                        onClicked: fileDialog.open()
                    }
                    
                    Button {
                        text: "Generate Colors"
                        enabled: configManager.wallpaperPath !== ""
                        
                        background: Rectangle {
                            color: parent.enabled ? (parent.pressed ? "#7287fd" : (parent.hovered ? "#89b4fa" : "#74c7ec")) : "#313244"
                            radius: 8
                        }
                        
                        contentItem: Text {
                            text: parent.text
                            color: parent.enabled ? "#1e1e2e" : "#6c7086"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.bold: true
                        }
                        
                        onClicked: {
                            wallpaperManager.generateColors(configManager.wallpaperPath)
                        }
                    }
                }
                
                SettingsRow {
                    label: "Wallpaper Mode"
                    description: "How to scale the wallpaper"
                    
                    ComboBox {
                        model: ["fill", "fit", "stretch", "center", "tile"]
                        currentIndex: model.indexOf(configManager.wallpaperMode)
                        onActivated: configManager.wallpaperMode = currentText
                        
                        background: Rectangle {
                            color: "#313244"
                            radius: 6
                            border.color: "#45475a"
                            border.width: 1
                        }
                        
                        contentItem: Text {
                            text: parent.displayText
                            color: "#cdd6f4"
                            verticalAlignment: Text.AlignVCenter
                            leftPadding: 10
                        }
                    }
                }
            }
        }
        
        Item { Layout.fillHeight: true }
    }
    
    FileDialog {
        id: fileDialog
        title: "Select Wallpaper"
        nameFilters: ["Images (*.png *.jpg *.jpeg *.webp)"]
        onAccepted: {
            let path = selectedFile.toString().replace("file://", "")
            wallpaperManager.setWallpaper(path)
            configManager.wallpaperPath = path
        }
    }
    
    Connections {
        target: wallpaperManager
        function onColorsGenerated() {
            // Show notification
            notification.text = "Colors generated! Restart shell to apply."
            notification.visible = true
            notificationTimer.restart()
        }
    }
    
    // Notification
    Rectangle {
        id: notification
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 20
        width: notifText.width + 40
        height: 50
        radius: 8
        color: "#a6e3a1"
        visible: false
        
        property alias text: notifText.text
        
        Text {
            id: notifText
            anchors.centerIn: parent
            color: "#1e1e2e"
            font.pixelSize: 14
            font.bold: true
        }
        
        Timer {
            id: notificationTimer
            interval: 3000
            onTriggered: notification.visible = false
        }
    }
}
