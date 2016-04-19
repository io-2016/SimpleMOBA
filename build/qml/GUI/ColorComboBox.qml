import QtQuick 2.0

FocusScope {
    property var colors

    id: menu
    width: selectorWrap.width
    height: selectorWrap.height

    onFocusChanged: {
        if (!focus)
            dropDown.visible = false
    }

    Item {
        id: selectorWrap
        width: selectorColumn.width
        height: selectorColumn.height

        property size delegateSize: Qt.size(100, 20)

        Column {
            id: selectorColumn
            width: dropDown.width
            height: (dropDown.visible ? dropDown.height : 0) + selected.height

            Item {
                width: selectorWrap.delegateSize.width
                height: selectorWrap.delegateSize.height
                Rectangle {
                    id: selected
                    color: colors[0]
                    anchors.fill: parent
                    anchors.margins: 1
                    border.width: 1

                    MouseArea {
                        z: 1
                        width: parent.height
                        height: width
                        anchors.right: parent.right

                        Rectangle {
                            anchors.fill: parent
                            color: "black"
                        }

                        onClicked: {
                            dropDown.visible = !dropDown.visible
                            menu.focus = true
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: dropDown.visible = false
                    }
                }
            }

            Column {
                id: dropDown
                visible: false
                width: colorsRepeater.width
                height: colorsRepeater.height

                Repeater {
                    id: colorsRepeater
                    model: colors
                    width: selectorWrap.delegateSize.width
                    height: count * selectorWrap.delegateSize.height

                    delegate: Item {
                        width: menu.width
                        height: selectorWrap.delegateSize.height

                        Rectangle {
                            id: colorBox
                            color: modelData
                            anchors.fill: parent
                            anchors.margins: 1
                            border.width: 1

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    selected.color = colorBox.color
                                    dropDown.visible = false
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
