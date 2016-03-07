import QtQuick 2.2

Item {
    anchors.fill: parent

    Text {
        id: fpsText
        anchors.right: parent.right
        color: "#AABBBB"
        text: "FPS: " + Math.round(world.fps)
    }

    Column {
        transformOrigin: Qt.TopLeftCorner
        anchors.top: fpsText.bottom
        anchors.right: parent.right

        Text {
            id: actionName
            anchors.right: parent.right
            color: "#AAAAAA"
            text: mapEditor.currentAction
        }

        Text {
            id: subactionName
            anchors.right: parent.right
            color: "#AABBBB"
            text: addBody.currentAction
        }
    }

}
