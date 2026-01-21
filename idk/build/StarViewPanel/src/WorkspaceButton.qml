import QtQuick
import QtQuick.Controls

Item {
    id: root
    width: 32
    height: 32
    
    property int number: 1
    property bool active: false
    
    signal clicked()
    
    Rectangle {
        id: glow
        anchors.centerIn: circle
        width: circle.width + 20
        height: circle.height + 20
        radius: width / 2
        opacity: active ? 0.5 : 0
        visible: active
        
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#8089b4fa" }
            GradientStop { position: 1.0; color: "#0089b4fa" }
        }
        
        Behavior on opacity {
            NumberAnimation { duration: 300; easing.type: Easing.OutCubic }
        }
        
        SequentialAnimation on scale {
            running: active
            loops: Animation.Infinite
            PropertyAnimation { from: 1.0; to: 1.2; duration: 1000; easing.type: Easing.InOutQuad }
            PropertyAnimation { from: 1.2; to: 1.0; duration: 1000; easing.type: Easing.InOutQuad }
        }
    }
    
    Rectangle {
        id: circle
        anchors.centerIn: parent
        width: parent.width
        height: parent.height
        radius: width / 2
        color: active ? "#89b4fa" : "transparent"
        border.color: active ? "transparent" : "#94a3b8"
        border.width: active ? 0 : 2
        
        Behavior on color { ColorAnimation { duration: 200 } }
        Behavior on border.color { ColorAnimation { duration: 200 } }
        Behavior on scale { NumberAnimation { duration: 300; easing.type: Easing.InOutQuad } }
        
        Gradient {
            id: activeGradient
            GradientStop { position: 0.0; color: "#89b4fa" }
            GradientStop { position: 1.0; color: "#7287fd" }
        }
        
        gradient: active ? activeGradient : null
    }
    
    Text {
        anchors.centerIn: circle
        text: root.number
        font.pixelSize: 12
        font.bold: true
        color: active ? "#1e1e2e" : "#cdd6f4"
        z: 10
        Behavior on color { ColorAnimation { duration: 200 } }
    }
    
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        
        onEntered: {
            if (!active) {
                circle.scale = 1.1
                circle.border.color = "#cbd5e1"
            }
        }
        
        onExited: {
            circle.scale = 1.0
            if (!active) circle.border.color = "#94a3b8"
        }
        
        onPressed: { circle.scale = 0.95 }
        onReleased: { circle.scale = active ? 1.0 : 1.1 }
        onClicked: { root.clicked() }
    }
    
    onActiveChanged: {
        if (active) {
            circle.scale = 1.15
            activateAnimation.start()
        } else {
            circle.scale = 1.0
        }
    }
    
    SequentialAnimation {
        id: activateAnimation
        NumberAnimation { target: circle; property: "scale"; to: 1.25; duration: 150; easing.type: Easing.OutCubic }
        NumberAnimation { target: circle; property: "scale"; to: 1.15; duration: 300; easing.type: Easing.InOutQuad }
    }
}
