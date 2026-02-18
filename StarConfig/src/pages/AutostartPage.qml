import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    ScrollView { anchors.fill: parent; contentWidth: availableWidth
        ColumnLayout { width: parent.width - 60; x: 30; spacing: 20
            Text { text: "Autostart"; font.pixelSize: 26; font.bold: true; color: root.cText; Layout.topMargin: 30 }

            SettingsGroup { title: "Add Command"
                RowLayout { Layout.fillWidth:true; spacing:8
                    TextField { id:_nc; Layout.fillWidth:true; placeholderText:"Command (e.g. waybar)"; font.pixelSize:12; color:root.cText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        onAccepted: _acb.clicked() }
                    Rectangle { id:_acb; width:60; height:36; radius:6; color:_acma.containsMouse?Qt.lighter(root.cAccent,1.1):root.cAccent
                        signal clicked()
                        Text { anchors.centerIn:parent; text:"Add"; font.pixelSize:13; color:root.cOnAccent }
                        MouseArea { id:_acma; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor
                            onClicked: { if(_nc.text){ configManager.addAutostart(_nc.text); _nc.text="" }; parent.clicked() } } }
                }
            }

            SettingsGroup { title: "Startup Commands ("+configManager.autostart.length+")"
                Repeater { model: configManager.autostart
                    RowLayout { Layout.fillWidth:true; spacing:10
                        Text { text:modelData; color:root.cText; font.pixelSize:13; font.family:"monospace"; Layout.fillWidth:true; elide:Text.ElideRight }
                        Text { text:"✕"; color:root.cError; font.pixelSize:16
                            MouseArea { anchors.fill:parent; anchors.margins:-6; cursorShape:Qt.PointingHandCursor; onClicked:configManager.removeAutostart(index) } }
                    }
                }
            }
            Item { height: 30 }
        }
    }
}
