import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import "../components"

Item {
    id: wpPage
    property string pendingImage: configManager.bgImage || ""

    Connections {
        target: configManager
        function onConfigChanged() {
            // Only update from configManager if we don't have a pending local change
            if (wpPage.pendingImage === "" || wpPage.pendingImage === configManager.bgImage)
                wpPage.pendingImage = configManager.bgImage || ""
        }
    }

    ScrollView { anchors.fill: parent; contentWidth: availableWidth
        ColumnLayout { width: parent.width - 60; x: 30; spacing: 20
            Text { text: "Wallpaper & Colors"; font.pixelSize: 26; font.bold: true; color: root.cText; Layout.topMargin: 30 }

            SettingsGroup { title: "Background"
                SettingsRow { label: "Enable Background"
                    Switch { checked:configManager.bgEnabled; onCheckedChanged: configManager.bgEnabled=checked } }
                SettingsRow { label: "Background Color"
                    TextField { text:configManager.bgColor; implicitWidth:120; font.pixelSize:12; color:root.cText
                        background: Rectangle{color:root.cCrust;radius:6;border.color:root.cSurfaceVar;border.width:1}
                        onEditingFinished: configManager.bgColor=text } }
                SettingsRow { label: "Mode"
                    StyledComboBox { model:["fill","fit","stretch","center","tile","color"]
                        currentIndex: model.indexOf(configManager.bgMode); onActivated: configManager.bgMode=currentText } }
            }

            SettingsGroup { title: "Wallpaper Image"
                Rectangle { Layout.fillWidth:true; Layout.preferredHeight:220; radius:8; color:root.cCrust; clip:true
                    Image {
                        anchors.fill:parent; anchors.margins:2; fillMode:Image.PreserveAspectCrop
                        source: wpPage.pendingImage ? "file://" + wpPage.pendingImage : ""
                        cache: false
                        asynchronous: true
                    }
                    Text { anchors.centerIn:parent; text:"No wallpaper set"; color:root.cSubtext; visible: !wpPage.pendingImage }
                }

                RowLayout { Layout.fillWidth:true; spacing:8
                    Rectangle { Layout.fillWidth:true; height:40; radius:8; color:_selma.containsMouse?root.cCrust:root.cMantle; border.color:root.cSurfaceVar; border.width:1
                        Text { anchors.centerIn:parent; text:"Select Image"; font.pixelSize:13; color:root.cText }
                        MouseArea { id:_selma; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor; onClicked:_fd.open() } }
                    Rectangle { Layout.fillWidth:true; height:40; radius:8
                        color: wpPage.pendingImage!=="" ? (_genma.containsMouse?Qt.lighter(root.cAccent,1.1):root.cAccent) : root.cCrust
                        Text { anchors.centerIn:parent; text:"Generate Matugen Colors"; font.pixelSize:13; font.bold:true
                               color: wpPage.pendingImage!=="" ? root.cOnAccent : root.cSubtext }
                        MouseArea { id:_genma; anchors.fill:parent; hoverEnabled:true; cursorShape:Qt.PointingHandCursor
                                    enabled: wpPage.pendingImage!==""; onClicked: colors.generateFromWallpaper(wpPage.pendingImage) } }
                }
                Text { text: wpPage.pendingImage||"No image selected"; font.pixelSize:11; color:root.cSubtext; elide:Text.ElideMiddle; Layout.fillWidth:true }
            }

            // Palette preview
            SettingsGroup { title: "Color Palette"; visible: colors.loaded
                GridLayout { Layout.fillWidth:true; columns:6; columnSpacing:6; rowSpacing:6
                    Repeater { model: [
                        {n:"Primary",c:colors.primary},{n:"Secondary",c:colors.secondary},{n:"Tertiary",c:colors.tertiary},
                        {n:"Error",c:colors.error},{n:"BG",c:colors.background},{n:"Surface",c:colors.surface}
                    ]
                    delegate: ColumnLayout { spacing:4
                        Rectangle { width:60;height:36;radius:6;color:modelData.c;border.color:root.cSurfaceVar;border.width:1 }
                        Text { text:modelData.n; font.pixelSize:9; color:root.cSubtext; Layout.alignment:Qt.AlignHCenter }
                    }}
                }
            }
            Item { height: 30 }
        }
    }
    FileDialog { id:_fd; title:"Select Wallpaper"; nameFilters:["Images (*.png *.jpg *.jpeg *.webp *.bmp)"]
        onAccepted: {
            var p = selectedFile.toString().replace("file://","")
            wpPage.pendingImage = p
            configManager.bgImage = p
        }
    }
}
