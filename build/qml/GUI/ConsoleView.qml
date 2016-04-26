import QtQuick 2.2

Rectangle {
    property int margin: 5

    id: rect
    x: 0.05 * root.width
    y: 0.05 * root.height
    clip: true
    color: "grey"
    radius: 10

    function hide() { width = height = 0 }

    function show() {
        width = Qt.binding(function() {
            return consoleMenu.width + 2 * margin
        })
        height = Qt.binding(function() {
            return consoleMenu.height + 2 * margin
        })
        visible = true
    }

    function toggle() {
        if (width === 0 && height === 0)
            show()
        else
            hide()
    }

    function updateHidden() {
        if (width === 0 || height === 0)
            visible = false
    }

    Behavior on width {
        SequentialAnimation {
            NumberAnimation { duration: 200 }
            ScriptAction { script: rect.updateHidden() }
        }
    }

    Behavior on height {
        SequentialAnimation {
            NumberAnimation { duration: 200 }
            ScriptAction { script: rect.updateHidden() }
        }
    }

    MouseArea { anchors.fill: parent }

    Console {
        id: consoleMenu
        x: rect.margin
        y: rect.margin
    }
}
