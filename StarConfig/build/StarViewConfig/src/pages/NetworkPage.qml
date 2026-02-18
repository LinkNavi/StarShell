import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

ScrollView {
    id: root
    objectName: "network"
    
    ColumnLayout {
        width: root.width - 40
        spacing: 20
        
        Text {
            text: "Network Settings"
            font.pixelSize: 28
            font.bold: true
            color: "#cdd6f4"
            Layout.topMargin: 30
            Layout.leftMargin: 30
        }
        
        SettingsGroup {
            title: "WiFi"
            Layout.fillWidth: true
            Layout.margins: 30
            
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 15
                
                // WiFi toggle
                SettingsRow {
                    label: "WiFi Enabled"
                    
                    Switch {
                        checked: networkManager.wifiEnabled
                        onToggled: networkManager.toggleWifi()
                    }
                }
                
                // Scan button
                Button {
                    text: "Scan Networks"
                    Layout.fillWidth: true
                    enabled: networkManager.wifiEnabled
                    
                    background: Rectangle {
                        color: parent.enabled ? (parent.pressed ? "#313244" : (parent.hovered ? "#45475a" : "#1e1e2e")) : "#11111b"
                        radius: 8
                        border.color: "#45475a"
                        border.width: 1
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        color: parent.enabled ? "#cdd6f4" : "#6c7086"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    
                    onClicked: {
                        var networks = networkManager.scanNetworks()
                        networkList.model = networks
                    }
                }
                
                // Network list
                ListView {
                    id: networkList
                    Layout.fillWidth: true
                    Layout.preferredHeight: 400
                    clip: true
                    spacing: 8
                    
                    delegate: Rectangle {
                        width: networkList.width
                        height: 60
                        radius: 8
                        color: mouseArea.containsMouse ? "#313244" : "#1e1e2e"
                        border.color: "#45475a"
                        border.width: 1
                        
                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 15
                            spacing: 15
                            
                            // Signal strength indicator
                            Text {
                                text: modelData.signal > 75 ? "📶" : (modelData.signal > 50 ? "📶" : (modelData.signal > 25 ? "📶" : "📶"))
                                font.pixelSize: 20
                                color: "#cdd6f4"
                            }
                            
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 4
                                
                                Text {
                                    text: modelData.ssid
                                    font.pixelSize: 14
                                    font.bold: true
                                    color: "#cdd6f4"
                                }
                                
                                Text {
                                    text: modelData.security || "Open"
                                    font.pixelSize: 11
                                    color: "#6c7086"
                                }
                            }
                            
                            Text {
                                text: modelData.signal + "%"
                                font.pixelSize: 12
                                color: "#89b4fa"
                            }
                        }
                        
                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            
                            onClicked: {
                                connectDialog.ssid = modelData.ssid
                                connectDialog.needsPassword = modelData.security !== ""
                                connectDialog.open()
                            }
                        }
                    }
                }
            }
        }
        
        Item { Layout.fillHeight: true }
    }
    
    // Connection dialog
    Dialog {
        id: connectDialog
        title: "Connect to " + ssid
        modal: true
        anchors.centerIn: parent
        
        property string ssid: ""
        property bool needsPassword: true
        
        background: Rectangle {
            color: "#1e1e2e"
            radius: 12
            border.color: "#45475a"
            border.width: 1
        }
        
        ColumnLayout {
            spacing: 15
            
            Text {
                text: "Password:"
                color: "#cdd6f4"
                visible: connectDialog.needsPassword
            }
            
            TextField {
                id: passwordField
                Layout.preferredWidth: 300
                echoMode: TextInput.Password
                placeholderText: "Enter password"
                visible: connectDialog.needsPassword
                
                background: Rectangle {
                    color: "#313244"
                    radius: 6
                    border.color: "#45475a"
                    border.width: 1
                }
                
                color: "#cdd6f4"
            }
            
            RowLayout {
                Layout.alignment: Qt.AlignRight
                spacing: 10
                
                Button {
                    text: "Cancel"
                    onClicked: connectDialog.close()
                    
                    background: Rectangle {
                        color: parent.pressed ? "#313244" : (parent.hovered ? "#45475a" : "#1e1e2e")
                        radius: 6
                        border.color: "#45475a"
                        border.width: 1
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        color: "#cdd6f4"
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
                
                Button {
                    text: "Connect"
                    
                    background: Rectangle {
                        color: parent.pressed ? "#7287fd" : (parent.hovered ? "#89b4fa" : "#74c7ec")
                        radius: 6
                    }
                    
                    contentItem: Text {
                        text: parent.text
                        color: "#1e1e2e"
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                    }
                    
                    onClicked: {
                        networkManager.connectToNetwork(
                            connectDialog.ssid,
                            passwordField.text
                        )
                        connectDialog.close()
                    }
                }
            }
        }
    }
}
