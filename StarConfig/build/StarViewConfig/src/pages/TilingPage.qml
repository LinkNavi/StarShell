import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../components"

Item {
    ScrollView { anchors.fill: parent; contentWidth: availableWidth
        ColumnLayout { width: parent.width - 60; x: 30; spacing: 20
            Text { text: "Tiling"; font.pixelSize: 26; font.bold: true; color: root.cText; Layout.topMargin: 30 }

            SettingsGroup { title: "Master-Stack Layout"
                SettingsRow { label: "Master Count"
                    SpinBox { from:1; to:5; value:configManager.masterCount; onValueChanged: configManager.masterCount=value
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1} } }

                SettingsRow { label: "Master Ratio"
                    RowLayout { spacing: 10
                        Slider { id:_rs; from:0.1; to:0.9; stepSize:0.05; value:configManager.masterRatio; implicitWidth:180
                            onMoved: configManager.masterRatio=value }
                        Text { text:configManager.masterRatio.toFixed(2); font.pixelSize:13; color:root.cSubtext }
                    }
                }
            }
            Item { height: 30 }
        }
    }
}
