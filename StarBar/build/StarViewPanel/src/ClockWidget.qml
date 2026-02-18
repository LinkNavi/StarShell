import QtQuick
import QtQuick.Layouts

Item {
    id: clockWidget
    implicitWidth: clockCol.width + 16

    property color fgColor: "#cdd6f4"
    property color dimColor: "#6c7086"
    property string clockFormat: "hh:mm"
    property bool showDate: false

    Column {
        id: clockCol
        anchors.centerIn: parent
        spacing: showDate ? -2 : 0

        Text {
            id: clockText
            anchors.horizontalCenter: parent.horizontalCenter
            text: Qt.formatTime(new Date(), clockWidget.clockFormat)
            font.pixelSize: showDate ? 12 : 13
            font.weight: Font.Medium
            color: clockWidget.fgColor
        }

        Text {
            visible: showDate
            anchors.horizontalCenter: parent.horizontalCenter
            text: Qt.formatDate(new Date(), "ddd MMM d")
            font.pixelSize: 9
            color: clockWidget.dimColor
        }
    }

    Timer {
        interval: 1000; running: true; repeat: true
        onTriggered: {
            var now = new Date()
            clockText.text = Qt.formatTime(now, clockWidget.clockFormat)
        }
    }
}
