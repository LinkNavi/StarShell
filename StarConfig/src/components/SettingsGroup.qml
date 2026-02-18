import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    
    property string title: ""
    property alias content: contentLoader.sourceComponent
    default property alias children: contentColumn.data
    
    color: "#11111b"
    radius: 12
    border.color: "#45475a"
    border.width: 1
    
    implicitHeight: contentColumn.height + 40
    
    ColumnLayout {
        id: contentColumn
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15
        
        Text {
            text: root.title
            font.pixelSize: 18
            font.bold: true
            color: "#89b4fa"
            visible: root.title !== ""
        }
    }
    
    Loader {
        id: contentLoader
        anchors.fill: parent
    }
}
