import QtQuick
import QtQuick.Layouts

Rectangle {
    id: sg
    property string title: ""
    default property alias children: _col.data
    Layout.fillWidth: true
    color: root.cMantle; radius: 12; border.color: root.cSurfaceVar; border.width: 1
    implicitHeight: _col.implicitHeight + 40
    ColumnLayout {
        id: _col
        anchors.left: parent.left; anchors.right: parent.right; anchors.top: parent.top
        anchors.margins: 20; spacing: 14
        Text { text: sg.title; font.pixelSize: 16; font.bold: true; color: root.cAccent; visible: sg.title !== "" }
    }
}
