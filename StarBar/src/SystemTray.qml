import QtQuick
import QtQuick.Layouts

Item {
    id: tray
    implicitWidth: trayRow.width + 8

    property color fgColor: "#cdd6f4"
    property color dimColor: "#6c7086"
    property color accentColor: "#89b4fa"

    property string batteryIcon: ""
    property string batteryText: ""
    property string volumeIcon: ""
    property string networkIcon: ""

    RowLayout {
        id: trayRow
        anchors.centerIn: parent
        spacing: 10

        // Network
        Text {
            text: tray.networkIcon || "󰤨"
            font.pixelSize: 14
            font.family: "Nerd Font,Font Awesome 6 Free,monospace"
            color: tray.fgColor
            visible: text !== ""
        }

        // Volume
        Text {
            text: tray.volumeIcon || "󰕾"
            font.pixelSize: 14
            font.family: "Nerd Font,Font Awesome 6 Free,monospace"
            color: tray.fgColor
            visible: text !== ""
        }

        // Battery
        Row {
            spacing: 4
            visible: tray.batteryText !== ""
            Text {
                text: tray.batteryIcon
                font.pixelSize: 14
                font.family: "Nerd Font,Font Awesome 6 Free,monospace"
                color: tray.fgColor
                anchors.verticalCenter: parent.verticalCenter
            }
            Text {
                text: tray.batteryText
                font.pixelSize: 11
                color: tray.dimColor
                anchors.verticalCenter: parent.verticalCenter
            }
        }
    }

    // Poll system info every 5 seconds (lightweight)
    Timer {
        interval: 5000; running: true; repeat: true; triggeredOnStart: true
        onTriggered: updateStatus()
    }

    function updateStatus() {
        // Battery: read from sysfs (no process spawn)
        var batPath = "/sys/class/power_supply/BAT0/capacity"
        var statusPath = "/sys/class/power_supply/BAT0/status"

        // We can't read files from QML directly without a helper,
        // but we set up defaults that the C++ side could override.
        // For now, leave empty - users without batteries won't see it.
    }
}
