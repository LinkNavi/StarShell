import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    id: animPg

    readonly property var animTypes: ["none","fade","slide","zoom","slide_fade"]
    readonly property var curveTypes: ["linear","ease_in","ease_out","ease_in_out","bounce","spring"]

    ScrollView { anchors.fill: parent; contentWidth: availableWidth
        ColumnLayout { width: parent.width - 60; x: 30; spacing: 20
            Text { text: "Animation"; font.pixelSize: 26; font.bold: true; color: root.cText; Layout.topMargin: 30 }

            SettingsGroup { title: "General"
                SettingsRow { label: "Enable Animations"
                    Switch { checked:configManager.animEnabled; onCheckedChanged: configManager.animEnabled=checked } }
                SettingsRow { label: "Duration (ms)"
                    RowLayout { spacing: 10
                        Slider { id:_durSlider; from:50; to:1000; stepSize:25; value:configManager.animDuration; implicitWidth:200
                            onMoved: configManager.animDuration=value }
                        Text { text:configManager.animDuration+"ms"; font.pixelSize:12; color:root.cSubtext; Layout.preferredWidth:50 }
                    }
                }
                SettingsRow { label: "Curve"
                    StyledComboBox { model:animPg.curveTypes; currentIndex: model.indexOf(configManager.animCurve)
                        onActivated: configManager.animCurve=currentText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        popup.background: Rectangle{color:root.cMantle;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
            }

            SettingsGroup { title: "Window Animations"
                SettingsRow { label: "Window Open"
                    StyledComboBox { model:animPg.animTypes; currentIndex: model.indexOf(configManager.animWindowOpen)
                        onActivated: configManager.animWindowOpen=currentText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        popup.background: Rectangle{color:root.cMantle;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
                SettingsRow { label: "Window Close"
                    StyledComboBox { model:animPg.animTypes; currentIndex: model.indexOf(configManager.animWindowClose)
                        onActivated: configManager.animWindowClose=currentText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        popup.background: Rectangle{color:root.cMantle;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
                SettingsRow { label: "Window Move"
                    StyledComboBox { model:animPg.animTypes; currentIndex: model.indexOf(configManager.animWindowMove)
                        onActivated: configManager.animWindowMove=currentText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        popup.background: Rectangle{color:root.cMantle;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
                SettingsRow { label: "Window Resize"
                    StyledComboBox { model:animPg.animTypes; currentIndex: model.indexOf(configManager.animWindowResize)
                        onActivated: configManager.animWindowResize=currentText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        popup.background: Rectangle{color:root.cMantle;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
            }

            SettingsGroup { title: "Workspace Animation"
                SettingsRow { label: "Workspace Switch"
                    StyledComboBox { model:animPg.animTypes; currentIndex: model.indexOf(configManager.animWorkspaceSwitch)
                        onActivated: configManager.animWorkspaceSwitch=currentText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        popup.background: Rectangle{color:root.cMantle;radius:6;border.color:root.cSurfaceVar;border.width:1} } }
            }

            SettingsGroup { title: "Fine-tuning"
                SettingsRow { label: "Fade Min Opacity"; desc: "Minimum opacity during fade (0.0 = fully transparent)"
                    RowLayout { spacing: 10
                        Slider { from:0.0; to:1.0; stepSize:0.05; value:configManager.animFadeMin; implicitWidth:180
                            onMoved: configManager.animFadeMin=value }
                        Text { text:configManager.animFadeMin.toFixed(2); font.pixelSize:12; color:root.cSubtext }
                    }
                }
                SettingsRow { label: "Zoom Min Scale"; desc: "Minimum scale during zoom (0.5 = half size)"
                    RowLayout { spacing: 10
                        Slider { from:0.0; to:1.0; stepSize:0.05; value:configManager.animZoomMin; implicitWidth:180
                            onMoved: configManager.animZoomMin=value }
                        Text { text:configManager.animZoomMin.toFixed(2); font.pixelSize:12; color:root.cSubtext }
                    }
                }
            }

            // Preview
            SettingsGroup { title: "Preview"
                Rectangle { Layout.fillWidth:true; Layout.preferredHeight:140; color:root.cCrust; radius:8; clip:true
                    Rectangle { id:_prev; width:80; height:60; radius:root.cAccent!=""?6:6; color:root.cAccent; x:20; y:40
                        Text { anchors.centerIn:parent; text:"Window"; font.pixelSize:10; color:root.cOnAccent }
                    }
                    Rectangle { Layout.fillWidth:false; width:100; height:36; radius:6; anchors.bottom:parent.bottom; anchors.bottomMargin:8; anchors.horizontalCenter:parent.horizontalCenter
                        color:_prevMa.containsMouse?Qt.lighter(root.cAccent,1.1):root.cAccent
                        Text { anchors.centerIn:parent; text:"Play"; font.pixelSize:13; font.bold:true; color:root.cOnAccent }
                        MouseArea { id:_prevMa; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor
                            onClicked: {
                                _prevAnim.duration = configManager.animDuration
                                _prev.opacity = configManager.animFadeMin
                                _prev.scale = configManager.animZoomMin
                                _prevAnim.restart()
                            }
                        }
                    }
                    ParallelAnimation { id:_prevAnim; property int duration: 200
                        NumberAnimation { target:_prev; property:"opacity"; from:configManager.animFadeMin; to:1.0; duration:_prevAnim.duration; easing.type:Easing.OutCubic }
                        NumberAnimation { target:_prev; property:"scale"; from:configManager.animZoomMin; to:1.0; duration:_prevAnim.duration; easing.type:Easing.OutCubic }
                    }
                }
            }

            Item { height: 30 }
        }
    }
}
