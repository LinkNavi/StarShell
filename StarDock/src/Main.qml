import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

ApplicationWindow {
    id: mainWindow
    visible: false
    width: 1920
    height: 80
    color: "transparent"
    flags: Qt.FramelessWindowHint
    
    // Running apps model
    property var runningApps: []
    
    Component.onCompleted: {
        updateRunningApps()
    }
    
    function updateRunningApps() {
        runningApps = dockController.getRunningApps()
    }
    
    // Connect to window tracker signals
    Connections {
        target: windowTracker
        function onRunningAppsChanged() {
            mainWindow.updateRunningApps()
        }
    }
    
    // Main dock container
    Rectangle {
        id: dockBackground
        anchors.centerIn: parent
        width: Math.max(200, appRow.width + launcherButton.width + 50)
        height: 70
        radius: 16
        color: "#1a1a2e"
        opacity: 0.85
        border.color: "#45475a"
        border.width: 1
        
        // Subtle inner glow
        Rectangle {
            anchors.fill: parent
            anchors.margins: 1
            radius: parent.radius - 1
            color: "transparent"
            border.color: "#313244"
            border.width: 1
        }
        
        // Background gradient overlay
        Rectangle {
            anchors.fill: parent
            radius: parent.radius
            opacity: 0.1
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#ffffff" }
                GradientStop { position: 1.0; color: "#000000" }
            }
        }
        
        layer.enabled: true
        layer.effect: MultiEffect {
            shadowEnabled: true
            shadowColor: "#90000000"
            shadowBlur: 1.0
            shadowHorizontalOffset: 0
            shadowVerticalOffset: 8
        }
        
        Row {
            anchors.centerIn: parent
            spacing: 8
            
            // App Launcher Button
            Item {
                id: launcherButton
                width: 60
                height: 60
                
                property bool isHovered: launcherMouse.containsMouse
                
                Rectangle {
                    anchors.centerIn: parent
                    width: launcherButton.isHovered ? 64 : 48
                    height: width
                    radius: 12
                    color: "#313244"
                    border.color: launcherButton.isHovered ? "#89b4fa" : "#45475a"
                    border.width: launcherButton.isHovered ? 2 : 1
                    
                    Behavior on width {
                        NumberAnimation {
                            duration: 200
                            easing.type: Easing.OutCubic
                        }
                    }
                    
                    Behavior on border.color {
                        ColorAnimation { duration: 200 }
                    }
                    
                    // Grid icon (app launcher)
                    Grid {
                        anchors.centerIn: parent
                        columns: 3
                        rows: 3
                        spacing: 3
                        
                        Repeater {
                            model: 9
                            Rectangle {
                                width: 4
                                height: 4
                                radius: 2
                                color: "#cdd6f4"
                            }
                        }
                    }
                    
                    // Hover glow
                    Rectangle {
                        anchors.fill: parent
                        radius: parent.radius
                        color: "#89b4fa"
                        opacity: launcherButton.isHovered ? 0.15 : 0
                        
                        Behavior on opacity {
                            NumberAnimation { duration: 200 }
                        }
                    }
                    
                    layer.enabled: launcherButton.isHovered
                    layer.effect: MultiEffect {
                        shadowEnabled: true
                        shadowColor: "#89b4fa"
                        shadowBlur: 0.8
                        shadowHorizontalOffset: 0
                        shadowVerticalOffset: 0
                    }
                }
                
                MouseArea {
                    id: launcherMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    
                    onClicked: {
                        dockController.showLauncher()
                    }
                }
                
                ToolTip {
                    visible: launcherMouse.containsMouse
                    delay: 500
                    text: "Applications"
                    
                    background: Rectangle {
                        color: "#1e1e2e"
                        radius: 6
                        border.color: "#45475a"
                        border.width: 1
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        color: "#cdd6f4"
                        font.pixelSize: 12
                    }
                }
            }
            
            // Separator
            Rectangle {
                width: 2
                height: 40
                color: "#45475a"
                opacity: 0.5
                anchors.verticalCenter: parent.verticalCenter
            }
            
            // Running apps row
            Row {
                id: appRow
                spacing: 8
                
                Repeater {
                    model: mainWindow.runningApps
                    
                    Item {
                        id: appContainer
                        width: 60
                        height: 60
                        
                        property bool isHovered: mouseArea.containsMouse
                        property int baseSize: 48
                        property int hoveredSize: 64
                        property var appData: modelData
                        
                        // App icon button
                        Rectangle {
                            id: iconButton
                            anchors.centerIn: parent
                            width: appContainer.isHovered ? appContainer.hoveredSize : appContainer.baseSize
                            height: width
                            radius: 12
                            color: "#313244"
                            border.color: appContainer.isHovered ? "#89b4fa" : (appData.focused ? "#a6e3a1" : "#45475a")
                            border.width: appContainer.isHovered ? 2 : 1
                            
                            Behavior on width {
                                NumberAnimation {
                                    duration: 200
                                    easing.type: Easing.OutCubic
                                }
                            }
                            
                            Behavior on border.color {
                                ColorAnimation { duration: 200 }
                            }
                            
                            // Icon image
                            Image {
                                anchors.centerIn: parent
                                width: parent.width * 0.6
                                height: width
                                source: dockController.getIconPath(appData.icon)
                                fillMode: Image.PreserveAspectFit
                                smooth: true
                                
                                // Fallback to text if icon not found
                                Text {
                                    anchors.centerIn: parent
                                    text: appData.name.substring(0, 2).toUpperCase()
                                    font.pixelSize: 18
                                    font.bold: true
                                    color: "#cdd6f4"
                                    visible: parent.status !== Image.Ready
                                }
                            }
                            
                            // Hover glow effect
                            Rectangle {
                                anchors.fill: parent
                                radius: parent.radius
                                color: "#89b4fa"
                                opacity: appContainer.isHovered ? 0.15 : 0
                                
                                Behavior on opacity {
                                    NumberAnimation { duration: 200 }
                                }
                            }
                            
                            layer.enabled: appContainer.isHovered
                            layer.effect: MultiEffect {
                                shadowEnabled: true
                                shadowColor: "#89b4fa"
                                shadowBlur: 0.8
                                shadowHorizontalOffset: 0
                                shadowVerticalOffset: 0
                            }
                        }
                        
                        // Active indicator dot
                        Rectangle {
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: -2
                            width: 4
                            height: 4
                            radius: 2
                            color: "#89b4fa"
                            opacity: 0.8
                        }
                        
                        // Mouse interaction
                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            acceptedButtons: Qt.LeftButton | Qt.RightButton
                            
                            onClicked: (mouse) => {
                                if (mouse.button === Qt.LeftButton) {
                                    // Focus/raise window (TODO: implement via IPC)
                                    console.log("Clicked:", appData.name)
                                } else if (mouse.button === Qt.RightButton) {
                                    // Show context menu (TODO)
                                }
                            }
                        }
                        
                        // Tooltip
                        ToolTip {
                            visible: mouseArea.containsMouse
                            delay: 500
                            text: appData.name
                            
                            background: Rectangle {
                                color: "#1e1e2e"
                                radius: 6
                                border.color: "#45475a"
                                border.width: 1
                                
                                layer.enabled: true
                                layer.effect: MultiEffect {
                                    shadowEnabled: true
                                    shadowColor: "#60000000"
                                    shadowBlur: 0.6
                                    shadowVerticalOffset: 4
                                }
                            }
                            
                            contentItem: Text {
                                text: parent.text
                                color: "#cdd6f4"
                                font.pixelSize: 12
                            }
                        }
                    }
                }
            }
        }
    }
}
