import QtQuick 2.0

Item {
    id: idHudContainer

    signal spellCast(int x, int y, int id)

    MouseArea {
        id: idMouseContainer
        anchors.fill: parent
        propagateComposedEvents: true
        enabled: false

        onClicked: {
            if (idSpellWrap.selected != -1) {
                spellCast(mouseX, mouseY, idSpellWrap.selected)
                idSpellWrap.selected = -1
            }
        }
    }

    Item {
        id: idHud
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: Math.min(idHudContainer.width, idHudContainer.height) * 0.2

        state: world.minimapOnLeft ? "minimapLeft" : "minimapRight"

        Minimap {
            id: idMinimap
            anchors.left: parent.left
            anchors.right: undefined
            anchors.bottom: parent.bottom
            height: parent.height
            width: height
        }

        Inventory {
            id: idInventory
            anchors.left: undefined
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: parent.height
        }

        Item {
            id: idBars
            anchors.left: idMinimap.right
            anchors.right: idInventory.left
            anchors.bottom: parent.bottom
            height: parent.height

            HealthBox {
                id: idHealthBox
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                height: parent.height / 2
                width: Math.min(
                           idHudContainer.width * 0.5,
                           idHudContainer.width
                           - idMinimap.width - idInventory.width)

            }

            Rectangle {
                id: idSpellBox
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: idHealthBox.bottom
                anchors.bottom: parent.bottom
                color: Qt.rgba(0.3, 0.3, 0.3, 0.3)
                width: Math.min(
                           idHudContainer.width * 0.5,
                           idHudContainer.width
                           - idMinimap.width - idInventory.width) - 4

                property var spellIcons: [
                    "/spell_icons/resources/enchant-blue-1.png",
                    "/spell_icons/resources/enchant-blue-2.png",
                    "/spell_icons/resources/enchant-blue-3.png",
                    "/spell_icons/resources/enchant-acid-1.png",
                    "/spell_icons/resources/enchant-acid-2.png",
                    "/spell_icons/resources/enchant-acid-3.png",
                ]

                Rectangle {
                    id: idStatBox
                    anchors.left: parent.left
                    anchors.top: parent.top

                    anchors.bottom: parent.bottom
                    width: parent.width / 3
                    anchors.margins: 4
                    color: Qt.rgba(0.3, 0.3, 0.3, 0.3)
                }

                Rectangle {
                    id: idSpellWrap
                    anchors.left: idStatBox.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.margins: 4
                    color: Qt.rgba(0.3, 0.3, 0.3, 0.3)

                    property int selected: -1

                    onSelectedChanged: {
                        if (selected === -1) {
                            idMouseContainer.enabled = false
                            idMouseContainer.cursorShape = Qt.ArrowCursor
                        } else {
                            idMouseContainer.enabled = true
                            idMouseContainer.cursorShape = Qt.CrossCursor
                        }
                    }

                    Row {
                        id: idSpellContainer
                        anchors.fill: parent
                        anchors.margins: 4
                        spacing: (width - height * (idSpellRepeater.model))
                                 / (idSpellRepeater.model - 1)

                        Repeater {
                            id: idSpellRepeater
                            model: 6

                            delegate: SpellFrame {
                                height: idSpellContainer.height
                                width: height
                                spellId: index
                                iconPath: idSpellBox.spellIcons[spellId]
                                selected: idSpellWrap.selected === index

                                onCast: {
                                    idSpellWrap.selected = spellId
                                }
                            }
                        }
                    }
                }
            }


        }
        states: [
            State {
                name: "minimapRight"

                AnchorChanges {
                    target: idMinimap
                    anchors.left: undefined
                    anchors.right: idHud.right
                }

                AnchorChanges {
                    target: idInventory
                    anchors.left: idHud.left
                    anchors.right: undefined
                }

                AnchorChanges {
                    target: idBars
                    anchors.left: idMinimap.right
                    anchors.right: idInventory.left
                }
            },
            State {
                name: "minimapLeft"

                AnchorChanges {
                    target: idMinimap
                    anchors.left: idHud.left
                    anchors.right: undefined
                }

                AnchorChanges {
                    target: idInventory
                    anchors.left: undefined
                    anchors.right: idHud.right
                }

                AnchorChanges {
                    target: idBars
                    anchors.left: idInventory.right
                    anchors.right: idMinimap.left
                }
            }
        ]
    }
}
