import QtQuick
import QtQuick.Layouts

RowLayout {
    id: sr
    property string label: ""
    property string desc: ""
    default property alias children: _ctrl.data
    Layout.fillWidth: true; spacing: 16
    ColumnLayout { Layout.fillWidth: true; spacing: 2
        Text { text: sr.label; font.pixelSize: 13; color: root.cText }
        Text { text: sr.desc; font.pixelSize: 11; color: root.cSubtext; visible: sr.desc !== "" }
    }
    Row { id: _ctrl; spacing: 8 }
}
