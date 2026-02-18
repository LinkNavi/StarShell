import QtQuick

Item {
    id: root
    width: Math.max(barHeight - 10, 24)
    height: width

    property int number: 1
    property bool active: false
    property color accentColor: "#89b4fa"
    property color fgColor: "#cdd6f4"
    property color bgColor: "#1e1e2e"
    property color inactiveColor: "#94a3b8"
    property int barHeight: 40

    signal clicked()

    // Indicator dot for active (minimal style)
    Rectangle {
        id: dot
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 1
        width: active ? 12 : 0
        height: 3
        radius: 1.5
        color: root.accentColor
        visible: active
        Behavior on width { NumberAnimation { duration: 150; easing.type: Easing.OutCubic } }
    }

    Rectangle {
        id: circle
        anchors.centerIn: parent
        width: parent.width - 4
        height: width
        radius: 6
        color: active ? Qt.rgba(root.accentColor.r, root.accentColor.g, root.accentColor.b, 0.2) : "transparent"

        Behavior on color { ColorAnimation { duration: 150 } }
        Behavior on scale { NumberAnimation { duration: 150; easing.type: Easing.OutCubic } }
    }

    Text {
        anchors.centerIn: circle
        text: root.number
        font.pixelSize: 11
        font.bold: active
        color: active ? root.accentColor : root.inactiveColor
        Behavior on color { ColorAnimation { duration: 150 } }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor

        onEntered: {
            if (!active) circle.color = Qt.rgba(1, 1, 1, 0.05)
        }
        onExited: {
            if (!active) circle.color = "transparent"
        }
        onPressed: circle.scale = 0.9
        onReleased: circle.scale = 1.0
        onClicked: root.clicked()
    }
}
