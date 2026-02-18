import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    id: monPg
    property var monitorList: []
    Component.onCompleted: monitorList = monitorManager.getMonitors()

    ScrollView { anchors.fill: parent; contentWidth: availableWidth
        ColumnLayout { width: parent.width - 60; x: 30; spacing: 20
            Text { text: "Monitors"; font.pixelSize: 26; font.bold: true; color: root.cText; Layout.topMargin: 30 }

            Rectangle { width:100; height:36; radius:6; color:_rfma.containsMouse?root.cCrust:root.cMantle; border.color:root.cSurfaceVar; border.width:1
                Text { anchors.centerIn:parent; text:"Refresh"; font.pixelSize:13; color:root.cText }
                MouseArea { id:_rfma; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor; onClicked: monPg.monitorList=monitorManager.getMonitors() } }

            Repeater { model: monPg.monitorList
                SettingsGroup { title: modelData.name||"Unknown"
                    SettingsRow { label:"Resolution"; Text { text:(modelData.width||"?")+"x"+(modelData.height||"?"); color:root.cText; font.pixelSize:13 } }
                    SettingsRow { label:"Refresh"; Text { text:(modelData.refresh||"?")+" Hz"; color:root.cText; font.pixelSize:13 } }
                    SettingsRow { label:"Scale"; Text { text:(modelData.scale||1.0).toFixed(1)+"x"; color:root.cText; font.pixelSize:13 } }
                    SettingsRow { label:"Enabled"; Switch { checked:modelData.enabled!==false; onCheckedChanged:monitorManager.setMonitorEnabled(modelData.name,checked) } }
                }
            }

            Text { text:"No monitors detected"; color:root.cSubtext; font.pixelSize:13; visible:monPg.monitorList.length===0; Layout.alignment:Qt.AlignHCenter }
            Item { height: 30 }
        }
    }
}
