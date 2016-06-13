import QtQuick 2.0

Item {
    Rectangle {
        anchors.fill: parent
        anchors.margins: 2
        radius: 2
        color: Qt.rgba(0.3, 0.3, 0.3, 0.5)
        border.width: 1
        MouseArea {
            property bool pressed: false
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onPressed: {
                if (mouse.button & Qt.LeftButton) {
                    pressed = true;
                    world.minimapMove(Qt.point(world.mapSize.width * mouse.x / width,
                                               world.mapSize.height * mouse.y / height));
                } else {
                    world.playerMove(Qt.point(world.mapSize.width * mouse.x / width,
                                              world.mapSize.height * mouse.y / height));
                }
            }
            onReleased: if (mouse.button & Qt.LeftButton) pressed = false
            onMouseXChanged: {
                if (pressed)
                    world.minimapMove(Qt.point(world.mapSize.width * mouse.x / width,
                                               world.mapSize.height * mouse.y / height));
            }
            onMouseYChanged: {
                if (pressed)
                    world.minimapMove(Qt.point(world.mapSize.width * mouse.x / width,
                                               world.mapSize.height * mouse.y / height));
            }
        }
    }

    Image {
        source: "qrc:/resources/dota_minimap.png"
        fillMode: Image.PreserveAspectFit
        width: parent.width
        height: parent.height
    }

    Rectangle {
        color: world.playerIndicatorColor
        width: parent.width * 0.05
        height: parent.height * 0.05
        radius: width * 0.5
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: parent.height * (world.playerLocation.y / world.mapSize.height) - radius
        anchors.leftMargin: parent.width * (world.playerLocation.x / world.mapSize.width) - radius
    }

    Rectangle {
        color: Qt.rgba(0, 0, 0, 0)
        width: parent.width * world.cameraLocation.width / world.mapSize.width
        height: parent.height * world.cameraLocation.height / world.mapSize.height
        border.color: Qt.rgba(1, 1, 1, 1)
        border.width: 1
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: parent.height * (world.cameraLocation.y / world.mapSize.height)
        anchors.leftMargin: parent.width * (world.cameraLocation.x / world.mapSize.width)
    }
}
