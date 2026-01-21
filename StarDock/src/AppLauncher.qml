import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects

Window {
    id: launcherWindow
    
    width: 600
    height: 500
    color: "transparent"
    flags: Qt.FramelessWindowHint
    
    // Center on screen
    Component.onCompleted: {
        x = (Screen.width - width) / 2
        y = (Screen.height - height) / 2
    }
    
    // Close on Escape
    Shortcut {
        sequence: "Escape"
        onActivated: launcherWindow.close()
    }
    
    // Backdrop
    Rectangle {
        anchors.fill: parent
        color: "#80000000"
        
        MouseArea {
            anchors.fill: parent
            onClicked: launcherWindow.close()
        }
    }
    
    // Launcher content
    Rectangle {
        id: launcherContent
        anchors.centerIn: parent
        width: 580
        height: 480
        radius: 16
        color: "#1a1a2e"
        opacity: 0.98
        border.color: "#45475a"
        border.width: 2
        
        layer.enabled: true
        layer.effect: MultiEffect {
            shadowEnabled: true
            shadowColor: "#c0000000"
            shadowBlur: 1.5
            shadowHorizontalOffset: 0
            shadowVerticalOffset: 12
        }
        
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 15
            
            // Header
            RowLayout {
                Layout.fillWidth: true
                spacing: 10
                
                Text {
                    text: "Applications"
                    font.pixelSize: 20
                    font.bold: true
                    color: "#cdd6f4"
                    Layout.fillWidth: true
                }
                
                Button {
                    text: "✕"
                    font.pixelSize: 16
                    flat: true
                    
                    contentItem: Text {
                        text: parent.text
                        color: "#cdd6f4"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    
                    background: Rectangle {
                        radius: 8
                        color: parent.hovered ? "#313244" : "transparent"
                    }
                    
                    onClicked: launcherWindow.close()
                }
            }
            
            // Search box
            TextField {
                id: searchField
                Layout.fillWidth: true
                placeholderText: "Search applications..."
                color: "#cdd6f4"
                font.pixelSize: 14
                
                background: Rectangle {
                    radius: 8
                    color: "#313244"
                    border.color: searchField.activeFocus ? "#89b4fa" : "#45475a"
                    border.width: 1
                }
                
                onTextChanged: {
                    appListModel.clear()
                    var apps = dockController.searchApps(text)
                    for (var i = 0; i < apps.length; i++) {
                        appListModel.append(apps[i])
                    }
                }
                
                Component.onCompleted: {
                    // Load all apps initially
                    var apps = dockController.getAllApps()
                    for (var i = 0; i < apps.length; i++) {
                        appListModel.append(apps[i])
                    }
                }
            }
            
            // App grid
            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                
                GridView {
                    id: appGrid
                    cellWidth: 120
                    cellHeight: 120
                    model: ListModel { id: appListModel }
                    
                    delegate: Item {
                        width: appGrid.cellWidth
                        height: appGrid.cellHeight
                        
                        Rectangle {
                            anchors.fill: parent
                            anchors.margins: 8
                            radius: 12
                            color: appMouseArea.containsMouse ? "#313244" : "transparent"
                            
                            Behavior on color {
                                ColorAnimation { duration: 150 }
                            }
                            
                            ColumnLayout {
                                anchors.centerIn: parent
                                spacing: 8
                                
                                // Icon
                                Rectangle {
                                    Layout.alignment: Qt.AlignHCenter
                                    width: 48
                                    height: 48
                                    radius: 8
                                    color: "#45475a"
                                    
                                    Image {
                                        anchors.centerIn: parent
                                        width: 36
                                        height: 36
                                        source: dockController.getIconPath(model.icon)
                                        fillMode: Image.PreserveAspectFit
                                        smooth: true
                                        
                                        Text {
                                            anchors.centerIn: parent
                                            text: model.name.substring(0, 2).toUpperCase()
                                            font.pixelSize: 16
                                            font.bold: true
                                            color: "#cdd6f4"
                                            visible: parent.status !== Image.Ready
                                        }
                                    }
                                }
                                
                                // Name
                                Text {
                                    Layout.alignment: Qt.AlignHCenter
                                    Layout.preferredWidth: 90
                                    text: model.name
                                    color: "#cdd6f4"
                                    font.pixelSize: 11
                                    elide: Text.ElideRight
                                    horizontalAlignment: Text.AlignHCenter
                                    wrapMode: Text.WordWrap
                                    maximumLineCount: 2
                                }
                            }
                            
                            MouseArea {
                                id: appMouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                
                                onClicked: {
                                    dockController.launchApp(model.exec)
                                    launcherWindow.close()
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
