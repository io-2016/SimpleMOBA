import QtQuick 2.2

Item {
    id: gameOverScreen
    visible: false
    anchors.fill: parent

    /*Connections {
        target: world
        onGameOver: gameOverScreen.visible = true
    }*/

    Text {
        anchors.centerIn: parent
        color: Qt.rgba(1, 0, 0)
        text: "Wasted"
        scale: 5
    }
}
