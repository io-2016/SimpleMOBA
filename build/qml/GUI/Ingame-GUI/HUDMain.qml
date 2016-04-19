import QtQuick 2.0

Item {
    id: idHudContainer

    Item {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: Math.min(idHudContainer.width, idHudContainer.height) * 0.2

        Minimap {
            id: idMinimap
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            height: parent.height
            width: height
        }

        Inventory {
            id: idInventory
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: parent.height
        }

        Item {
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
    }
}
