import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: panel
    
    visible: false
    width: 1920
    height: 40
    color: "transparent"
    flags: Qt.FramelessWindowHint
    
    property int currentWorkspace: 1
    property string focusedWindowTitle: ""
    
    // Background
    Rectangle {
        anchors.fill: parent
        color: "#1e1e2e"
        opacity: 0.95
        
        Rectangle {
            anchors.fill: parent
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#20ffffff" }
                GradientStop { position: 1.0; color: "#00ffffff" }
            }
        }
    }
    
    // Content
    RowLayout {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 20
        
        // LEFT: Workspace buttons
        Item {
            Layout.fillHeight: true
            Layout.preferredWidth: workspaceRow.width
            
            Row {
                id: workspaceRow
                anchors.centerIn: parent
                spacing: 8
                
                Repeater {
                    model: 9
                    
                    WorkspaceButton {
                        number: index + 1
                        active: panel.currentWorkspace === (index + 1)
                        onClicked: {
                            panel.currentWorkspace = number
                            ipcHandler.switchWorkspace(number)
                        }
                    }
                }
            }
        }
        
        // CENTER: Title (focused window or "StarView")
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            Rectangle {
                anchors.centerIn: parent
                width: titleText.width + 40
                height: parent.height - 10
                radius: 12
                color: "#313244"
                opacity: 0.6
                
                Rectangle {
                    anchors.fill: parent
                    radius: parent.radius
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "#20ffffff" }
                        GradientStop { position: 1.0; color: "#05ffffff" }
                    }
                }
                
                border.color: "#20ffffff"
                border.width: 1
                
                Text {
                    id: titleText
                    anchors.centerIn: parent
                    text: panel.focusedWindowTitle || "StarView"
                    font.pixelSize: 16
                    font.bold: true
                    color: "#cdd6f4"
                }
            }
        }
        
        // RIGHT: Clock
        Item {
            Layout.fillHeight: true
            Layout.preferredWidth: clockText.width + 20
            
            Text {
                id: clockText
                anchors.centerIn: parent
                text: Qt.formatTime(new Date(), "hh:mm")
                font.pixelSize: 14
                font.weight: Font.Medium
                color: "#cdd6f4"
            }
            
            Timer {
                interval: 1000
                running: true
                repeat: true
                onTriggered: clockText.text = Qt.formatTime(new Date(), "hh:mm")
            }
        }
    }
    
    // IPC handler connection
    Connections {
        target: ipcHandler
        function onWorkspaceChanged(ws) {
            panel.currentWorkspace = ws
            console.log("Workspace changed to:", ws)
        }
        function onFocusedWindowChanged(title) {
            panel.focusedWindowTitle = title
        }
    }
    
    Component.onCompleted: {
        console.log("Panel loaded!")
        ipcHandler.connect()
    }
}
