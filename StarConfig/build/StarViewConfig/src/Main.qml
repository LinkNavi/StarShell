import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    visible: true
    width: 1000
    height: 700
    title: "StarView Settings"
    
    color: "#1e1e2e"
    
    // Sidebar + content layout
    RowLayout {
        anchors.fill: parent
        spacing: 0
        
        // Sidebar
        Rectangle {
            Layout.fillHeight: true
            Layout.preferredWidth: 250
            color: "#11111b"
            
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 10
                
                // Title
                Text {
                    text: "Settings"
                    font.pixelSize: 24
                    font.bold: true
                    color: "#cdd6f4"
                    Layout.bottomMargin: 20
                }
                
                // Navigation buttons
                NavButton {
                    text: "General"
                    icon: "⚙️"
                    active: stackView.currentItem.objectName === "general"
                    onClicked: stackView.replace("pages/GeneralPage.qml")
                }
                
                NavButton {
                    text: "Decoration"
                    icon: "🎨"
                    active: stackView.currentItem.objectName === "decoration"
                    onClicked: stackView.replace("pages/DecorationPage.qml")
                }
                
                NavButton {
                    text: "Wallpaper"
                    icon: "🖼️"
                    active: stackView.currentItem.objectName === "wallpaper"
                    onClicked: stackView.replace("pages/WallpaperPage.qml")
                }
                
                NavButton {
                    text: "Monitors"
                    icon: "🖥️"
                    active: stackView.currentItem.objectName === "monitors"
                    onClicked: stackView.replace("pages/MonitorsPage.qml")
                }
                
                NavButton {
                    text: "Network"
                    icon: "📡"
                    active: stackView.currentItem.objectName === "network"
                    onClicked: stackView.replace("pages/NetworkPage.qml")
                }
                
                NavButton {
                    text: "Keybindings"
                    icon: "⌨️"
                    active: stackView.currentItem.objectName === "keybindings"
                    onClicked: stackView.replace("pages/KeybindingsPage.qml")
                }
                
                Item { Layout.fillHeight: true }
                
                // Apply button
                Button {
                    Layout.fillWidth: true
                    text: "Apply & Reload"
                    
                    background: Rectangle {
                        color: parent.pressed ? "#7287fd" : (parent.hovered ? "#89b4fa" : "#74c7ec")
                        radius: 8
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        color: "#1e1e2e"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 14
                        font.bold: true
                    }
                    
                    onClicked: {
                        configManager.save()
                        configManager.reloadCompositor()
                    }
                }
            }
        }
        
        // Content area
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#1e1e2e"
            
            StackView {
                id: stackView
                anchors.fill: parent
                initialItem: "pages/GeneralPage.qml"
            }
        }
    }
    
    // Navigation button component
    component NavButton: Rectangle {
        property string text
        property string icon
        property bool active: false
        signal clicked()
        
        Layout.fillWidth: true
        height: 50
        radius: 8
        color: active ? "#313244" : (mouseArea.containsMouse ? "#181825" : "transparent")
        
        Behavior on color { ColorAnimation { duration: 150 } }
        
        RowLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 12
            
            Text {
                text: parent.parent.icon
                font.pixelSize: 20
                color: "#cdd6f4"
            }
            
            Text {
                text: parent.parent.text
                font.pixelSize: 14
                color: active ? "#89b4fa" : "#cdd6f4"
                Layout.fillWidth: true
            }
        }
        
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.PointingHandCursor
            onClicked: parent.clicked()
        }
    }
}
