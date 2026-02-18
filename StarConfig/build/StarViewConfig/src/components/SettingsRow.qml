import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    id: root
    
    property string label: ""
    property string description: ""
    default property alias children: controlItem.data
    
    Layout.fillWidth: true
    spacing: 20
    
    ColumnLayout {
        Layout.fillWidth: true
        spacing: 4
        
        Text {
            text: root.label
            font.pixelSize: 14
            font.bold: true
            color: "#cdd6f4"
        }
        
        Text {
            text: root.description
            font.pixelSize: 11
            color: "#6c7086"
            visible: root.description !== ""
        }
    }
    
    Item {
        id: controlItem
        Layout.alignment: Qt.AlignRight
    }
}
