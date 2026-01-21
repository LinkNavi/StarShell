import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.kde.layershell 1.0 as LayerShell

ApplicationWindow {
    id: panel
    
    width: Screen.width
    height: 40
    visible: true
    color: "transparent"
    flags: Qt.FramelessWindowHint
    
    property int currentWorkspace: 1
    
    LayerShell.Window.layer: LayerShell.Window.LayerTop
    LayerShell.Window.anchors: LayerShell.Window.AnchorTop | 
                               LayerShell.Window.AnchorLeft | 
                               LayerShell.Window.AnchorRight
    LayerShell.Window.exclusionZone: 40
    
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
    
    RowLayout {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 20
        
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
                            console.log("Switched to workspace", number)
                        }
                    }
                }
            }
        }
        
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
                    text: "StarView"
                    font.pixelSize: 16
                    font.bold: true
                    color: "#cdd6f4"
                }
            }
        }
        
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
}
