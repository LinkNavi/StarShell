import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Effects
ApplicationWindow {
    visible: false
    width: 1920
    height: 40
    color: "transparent"
    flags: Qt.FramelessWindowHint
    
    // Background with gradient
    Rectangle {
        anchors.fill: parent
        color: "#1e1e2e"
        opacity: 0.95
	radius: 12
	border.color: "#89b4fa"
	border.width: 2
        Rectangle {
            anchors.fill: parent
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#20ffffff" }
                GradientStop { position: 1.0; color: "#00ffffff" }
            }
    }

  layer.enabled: true
    layer.effect: MultiEffect {
        shadowEnabled: true
        shadowColor: "#80000000"  // Semi-transparent black
        shadowBlur: 0.5
        shadowHorizontalOffset: 0
        shadowVerticalOffset: 4
    }
    }
    
    RowLayout {
        anchors.fill: parent
        anchors.margins: 5
        spacing: 20
        
        // Left section
        Text {
            text: "My Panel"
            color: "#cdd6f4"
            font.pixelSize: 14
        }
        
        // Center spacer
        Item { Layout.fillWidth: true }
        
        // Right section - Clock
        Text {
            id: clock
            text: Qt.formatTime(new Date(), "hh:mm")
            color: "#cdd6f4"
            font.pixelSize: 14
            
            Timer {
                interval: 1000
                running: true
                repeat: true
                onTriggered: clock.text = Qt.formatTime(new Date(), "hh:mm")
            }
        }
    }
}
