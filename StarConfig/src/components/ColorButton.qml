import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root

    property string currentColor: "#ffffff"
    signal colorChanged(string color)

    width: 80; height: 36
    radius: 6
    color: currentColor
    border.color: colors.outlineVariant
    border.width: 1

    Text {
        anchors.centerIn: parent
        text: root.currentColor
        font.pixelSize: 9
        color: {
            // Simple luminance check for contrast
            var c = root.currentColor
            if (c.charAt(0) === '#') c = c.substring(1)
            var r = parseInt(c.substring(0,2), 16) / 255
            var g = parseInt(c.substring(2,4), 16) / 255
            var b = parseInt(c.substring(4,6), 16) / 255
            var lum = 0.299*r + 0.587*g + 0.114*b
            return lum > 0.5 ? "#000000" : "#ffffff"
        }
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor
        onClicked: editPopup.open()
    }

    Popup {
        id: editPopup
        x: -10; y: parent.height + 4
        width: 220; height: 80
        padding: 10

        background: Rectangle {
            color: colors.surface
            radius: 8
            border.color: colors.outlineVariant
            border.width: 1
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 8

            TextField {
                id: hexInput
                Layout.fillWidth: true
                text: root.currentColor
                font.pixelSize: 13
                color: colors.onBackground
                placeholderText: "#RRGGBB"
                maximumLength: 9

                background: Rectangle {
                    color: colors.surfaceVariant
                    radius: 4
                    border.color: colors.outlineVariant
                    border.width: 1
                }

                onAccepted: {
                    var t = text.trim()
                    if (t.charAt(0) !== '#') t = '#' + t
                    root.currentColor = t
                    root.colorChanged(t)
                    editPopup.close()
                }
            }

            Button {
                Layout.fillWidth: true
                text: "Apply"
                height: 28
                background: Rectangle { color: colors.primary; radius: 4 }
                contentItem: Text { text: parent.text; color: colors.onPrimary; horizontalAlignment: Text.AlignHCenter; font.pixelSize: 12 }
                onClicked: hexInput.accepted()
            }
        }
    }
}
