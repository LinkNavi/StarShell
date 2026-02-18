import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    ScrollView { anchors.fill: parent; contentWidth: availableWidth
        ColumnLayout { width: parent.width - 60; x: 30; spacing: 20
            Text { text: "Window Rules"; font.pixelSize: 26; font.bold: true; color: root.cText; Layout.topMargin: 30 }

            SettingsGroup { title: "Add Rule"
                RowLayout { Layout.fillWidth:true; spacing:8
                    TextField { id:_raid; Layout.fillWidth:true; placeholderText:"app_id"; font.pixelSize:12; color:root.cText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} }
                    TextField { id:_rtitle; Layout.fillWidth:true; placeholderText:"title (optional)"; font.pixelSize:12; color:root.cText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} }
                }
                RowLayout { Layout.fillWidth:true; spacing:16
                    Text { text:"Floating"; color:root.cText; font.pixelSize:13 }
                    Switch { id:_rfloat }
                    Text { text:"Fullscreen"; color:root.cText; font.pixelSize:13 }
                    Switch { id:_rfs }
                    Text { text:"Workspace"; color:root.cText; font.pixelSize:13 }
                    SpinBox { id:_rws; from:0; to:10; value:0
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} }
                }
                Rectangle { Layout.fillWidth:true; height:36; radius:6; color:_arma.containsMouse?Qt.lighter(root.cAccent,1.1):root.cAccent
                    Text { anchors.centerIn:parent; text:"Add Rule"; font.pixelSize:13; color:root.cOnAccent }
                    MouseArea { id:_arma; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor
                        onClicked: {
                            if(!_raid.text&&!_rtitle.text) return
                            var r={}
                            if(_raid.text) r["app_id"]=_raid.text
                            if(_rtitle.text) r["title"]=_rtitle.text
                            if(_rfloat.checked) r["floating"]=true
                            if(_rfs.checked) r["fullscreen"]=true
                            if(_rws.value>0) r["workspace"]=_rws.value
                            configManager.addRule(r)
                            _raid.text=""; _rtitle.text=""; _rfloat.checked=false; _rfs.checked=false; _rws.value=0
                        }
                    }
                }
            }

            SettingsGroup { title: "Rules ("+configManager.rules.length+")"
                Repeater { model: configManager.rules
                    RowLayout { Layout.fillWidth:true; spacing:10
                        ColumnLayout { Layout.fillWidth:true; spacing:2
                            Text { text:(modelData.app_id||"*")+(modelData.title?" / "+modelData.title:""); color:root.cText; font.pixelSize:13 }
                            Text { text:[modelData.floating?"floating":"",modelData.fullscreen?"fullscreen":"",modelData.workspace?"ws:"+modelData.workspace:""].filter(function(s){return s}).join(", ")
                                   color:root.cSubtext; font.pixelSize:11 } }
                        Text { text:"✕"; color:root.cError; font.pixelSize:16
                            MouseArea { anchors.fill:parent; anchors.margins:-6; cursorShape:Qt.PointingHandCursor; onClicked:configManager.removeRule(index) } }
                    }
                }
            }
            Item { height: 30 }
        }
    }
}
