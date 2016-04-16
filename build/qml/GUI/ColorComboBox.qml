import QtQuick 2.0

FocusScope {
    property var colors

    id: menu
    width: 100
    height: 20

    onFocusChanged: {
        if (!focus)
            dropDown.visible = false
    }

    Rectangle {
        Column {
            Rectangle {
                id: selected
                width: menu.width
                height: menu.height
                border.width: 1
                color: colors[0]
                MouseArea {
                    z: 1
                    width: menu.height
                    height: menu.height
                    anchors.right: parent.right

                    Rectangle {
                        anchors.fill: parent
                        color: "black"
                    }

                    onClicked: {
                        dropDown.visible ^= 1
                        menu.focus = true
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: dropDown.visible = false
                }
            }
            Column {
                id: dropDown
                visible: false

                Repeater {
                    model: colors
                    Rectangle {
                        id: p
                        color: modelData
                        width: menu.width
                        height: menu.height
                        border.width: 1
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                selected.color = p.color
                                dropDown.visible = false
                            }
                        }
                    }
                }
            }
        }
    }
}
