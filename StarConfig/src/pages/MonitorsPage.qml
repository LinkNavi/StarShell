import QtQuick
import QtQuick.Controls
import "../components"

ScrollView {
    objectName: "monitors"
    
    Text {
        text: "Monitor Settings"
        font.pixelSize: 28
        font.bold: true
        color: "#cdd6f4"
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 30
    }
    
    Text {
        text: "Monitor configuration coming soon..."
        color: "#6c7086"
        anchors.centerIn: parent
    }
}
