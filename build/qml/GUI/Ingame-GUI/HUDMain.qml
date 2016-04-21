import QtQuick 2.0

Item {
    id: idHudContainer

    Item {
        id: idHud
        objectName: "idHud"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: Math.min(idHudContainer.width, idHudContainer.height) * 0.2

        state: app.minimapOnLeft ? "minimapLeft" : "minimapRight"

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
                anchors.horizontalCenter: parent.horizontalCenter
                height: parent.height
                width: Math.min(
                           idHudContainer.width * 0.5,
                           idHudContainer.width
                           - idMinimap.width - idInventory.width)

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
