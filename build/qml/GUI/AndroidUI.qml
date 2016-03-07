import QtQuick 2.2

Item {
    anchors.fill: parent

    Rectangle {
        width: 0.05*parent.width
        height: 0.05*parent.height
        radius: 5

        MultiPointTouchArea {
            anchors.fill: parent
            onPressed: mainMenu.toggle()
        }
    }

    Item {
        property int textMargin: 20

        id: playerOptions
        visible: world.player
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        height: parent.height*0.1

        Item {
            id: walking
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            width: parent.width*0.2
            height: parent.height

            Row {
                Rectangle {
                    id: left
                    border.color: "black"
                    width: Math.max(walking.width*0.5,
                                    leftText.contentWidth+playerOptions.textMargin)
                    height: Math.max(walking.height,
                                     leftText.contentHeight+playerOptions.textMargin)

                    Text {
                        id: leftText
                        text: "Left"
                        anchors.centerIn: parent
                    }
                }

                Rectangle {
                    id: right
                    border.color: left.border.color
                    width: Math.max(left.width,
                                    rightText.contentWidth+playerOptions.textMargin)
                    height: Math.max(left.height,
                                     rightText.contentHeight+playerOptions.textMargin)

                    Text {
                        id: rightText
                        text: "Right"
                        anchors.centerIn: parent
                    }
                }
            }

            MultiPointTouchArea {
                property bool onLeft

                anchors.fill: parent

                onPressed: {
                    var p = mapToItem(left, point.x, point.y)

                    if (left.contains(Qt.point(p.x, p.y))) {
                        world.playerEnableGoLeft()
                        onLeft = true
                    }
                    else {
                        world.playerEnableGoRight()
                        onLeft = false
                    }
                }

                onUpdated: {
                    if (!onLeft) {
                        var p1 = mapToItem(left, point.x, point.y)
                        if (left.contains(Qt.point(p1.x, p1.y))) {
                            world.playerDisableGoRight()
                            world.playerEnableGoLeft()
                            onLeft = true
                        }
                    }

                    if (onLeft) {
                        var p2 = mapToItem(right, point.x, point.y)
                        if (right.contains(Qt.point(p2.x, p2.y))) {
                            world.playerDisableGoLeft()
                            world.playerEnableGoRight()
                            onLeft = false
                        }
                    }
                }

                onReleased: {
                    world.playerDisableGoLeft()
                    world.playerDisableGoRight()
                }

                touchPoints: TouchPoint { id: point }
            }
        }

        Row {
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            Rectangle {
                width: Math.max(left.width,
                                punchText.contentWidth+playerOptions.textMargin)
                height: Math.max(left.height,
                                 punchText.contentHeight+playerOptions.textMargin)
                border.color: left.border.color

                Text {
                    id: punchText
                    anchors.centerIn: parent
                    text: "Punch"
                }

                MultiPointTouchArea {
                    anchors.fill: parent
                    onPressed: world.playerPunchRequested()
                }
            }

            Rectangle {
                width: Math.max(left.width,
                                dropText.contentWidth+playerOptions.textMargin)
                height: Math.max(left.height,
                                 dropText.contentHeight+playerOptions.textMargin)
                border.color: left.border.color
                visible: world.equippedWeapon

                Text {
                    id: dropText
                    anchors.centerIn: parent
                    text: "Drop"
                }

                MultiPointTouchArea {
                    anchors.fill: parent
                    onPressed: world.playerDropWeapon()
                }
            }

            Rectangle {
                width: Math.max(left.width,
                                jumpText.contentWidth+playerOptions.textMargin)
                height: Math.max(left.height,
                                 jumpText.contentHeight+playerOptions.textMargin)
                border.color: left.border.color

                Text {
                    id: jumpText
                    anchors.centerIn: parent
                    text: "Jump"
                }

                MultiPointTouchArea {
                    anchors.fill: parent
                    onPressed: world.playerJumpRequested()
                }
            }
        }
    }
}
