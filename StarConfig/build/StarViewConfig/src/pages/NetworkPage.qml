import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    id: netPg
    property var networkList: []

    ScrollView { anchors.fill: parent; contentWidth: availableWidth
        ColumnLayout { width: parent.width - 60; x: 30; spacing: 20
            Text { text: "Network"; font.pixelSize: 26; font.bold: true; color: root.cText; Layout.topMargin: 30 }

            SettingsGroup { title: "WiFi"
                SettingsRow { label: "WiFi Enabled"
                    Switch { checked:networkManager.wifiEnabled; onCheckedChanged: networkManager.toggleWifi() } }

                Rectangle { Layout.fillWidth:true; height:36; radius:6; color:_scma.containsMouse?root.cCrust:root.cMantle; border.color:root.cSurfaceVar; border.width:1
                    Text { anchors.centerIn:parent; text:"Scan Networks"; font.pixelSize:13; color: networkManager.wifiEnabled?root.cText:root.cSubtext }
                    MouseArea { id:_scma; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor; enabled:networkManager.wifiEnabled
                        onClicked: netPg.networkList=networkManager.scanNetworks() } }

                Repeater { model: netPg.networkList
                    RowLayout { Layout.fillWidth:true; spacing:12
                        Text { text:"📶"; font.pixelSize:16 }
                        ColumnLayout { Layout.fillWidth:true; spacing:2
                            Text { text:modelData.ssid||"Hidden"; color:root.cText; font.pixelSize:13 }
                            Text { text:(modelData.security||"Open")+" · "+modelData.signal+"%"; color:root.cSubtext; font.pixelSize:11 } }
                        Rectangle { width:80; height:30; radius:6; color:_cnma.containsMouse?Qt.lighter(root.cAccent,1.1):root.cAccent
                            Text { anchors.centerIn:parent; text:"Connect"; font.pixelSize:12; color:root.cOnAccent }
                            MouseArea { id:_cnma; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor
                                onClicked: { _cd.ssid=modelData.ssid; _cd.secured=(modelData.security||"")!==""; _cd.open() } } }
                    }
                }
            }
            Item { height: 30 }
        }
    }

    Dialog { id:_cd; anchors.centerIn:parent; modal:true; property string ssid:""; property bool secured:true
        background: Rectangle{color:root.cMantle;radius:12;border.color:root.cSurfaceVar;border.width:1}
        header: Text{text:"Connect to "+_cd.ssid;color:root.cText;font.pixelSize:16;font.bold:true;padding:16}
        ColumnLayout { spacing:12; width:300
            TextField { id:_pw; Layout.fillWidth:true; visible:_cd.secured; echoMode:TextInput.Password; placeholderText:"Password"
                font.pixelSize:12; color:root.cText; background:Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} }
            RowLayout { Layout.alignment:Qt.AlignRight; spacing:8
                Rectangle { width:70;height:32;radius:6;color:_ccma.containsMouse?root.cCrust:"transparent"
                    Text{anchors.centerIn:parent;text:"Cancel";font.pixelSize:13;color:root.cText}
                    MouseArea{id:_ccma;anchors.fill:parent;hoverEnabled:true;cursorShape:Qt.PointingHandCursor;onClicked:_cd.close()} }
                Rectangle { width:80;height:32;radius:6;color:_cxma.containsMouse?Qt.lighter(root.cAccent,1.1):root.cAccent
                    Text{anchors.centerIn:parent;text:"Connect";font.pixelSize:13;font.bold:true;color:root.cOnAccent}
                    MouseArea{id:_cxma;anchors.fill:parent;hoverEnabled:true;cursorShape:Qt.PointingHandCursor
                        onClicked:{networkManager.connectToNetwork(_cd.ssid,_pw.text);_cd.close()}} }
            }
        }
    }
}
