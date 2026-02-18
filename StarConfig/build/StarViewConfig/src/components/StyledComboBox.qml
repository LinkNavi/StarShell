import QtQuick
import QtQuick.Controls

ComboBox {
    id: control
    implicitHeight: 36

    contentItem: control.editable ? _editLoader.item : _textDisplay
    Component.onCompleted: if (control.editable) _editLoader.active = true

    Text {
        id: _textDisplay
        visible: !control.editable
        leftPadding: 10; rightPadding: control.indicator.width + 10
        text: control.displayText
        font.pixelSize: 12
        color: root.cText
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    Loader {
        id: _editLoader
        active: false
        sourceComponent: TextInput {
            leftPadding: 10; rightPadding: control.indicator.width + 10
            text: control.editText
            font.pixelSize: 12
            color: root.cText
            selectionColor: root.cAccent
            selectedTextColor: root.cOnAccent
            verticalAlignment: Text.AlignVCenter
            onTextEdited: control.editText = text
        }
    }

    indicator: Text {
        x: control.width - width - 10
        y: (control.height - height) / 2
        text: "▾"; font.pixelSize: 12; color: root.cSubtext
    }

    background: Rectangle {
        color: control.pressed ? Qt.lighter(root.cCrust, 1.1) : root.cCrust
        radius: 6
        border.color: control.activeFocus ? root.cAccent : root.cSurfaceVar
        border.width: 1
    }

    delegate: ItemDelegate {
        width: control.popup.width
        height: 34
        contentItem: Text {
            text: control.textRole ? (Array.isArray(control.model) ? modelData : model[control.textRole]) : modelData
            font.pixelSize: 12
            color: highlighted ? root.cOnAccent : root.cText
            verticalAlignment: Text.AlignVCenter
            leftPadding: 10
        }
        background: Rectangle {
            color: highlighted ? root.cAccent : (hovered ? Qt.rgba(root.cAccent.r, root.cAccent.g, root.cAccent.b, 0.1) : "transparent")
            radius: 4
        }
        highlighted: control.highlightedIndex === index
        hoverEnabled: true
    }

    popup: Popup {
        y: control.height + 2
        width: control.width
        implicitHeight: Math.min(contentItem.implicitHeight + 8, 300)
        padding: 4

        background: Rectangle {
            color: root.cMantle
            radius: 8
            border.color: root.cSurfaceVar
            border.width: 1
        }

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex
            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }
        }
    }
}
