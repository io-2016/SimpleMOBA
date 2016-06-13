import QtQuick 2.0

Item {
    Rectangle {
        anchors.fill: parent
        anchors.margins: 2
        color: Qt.rgba(0.3, 0.3, 0.3, 1.0)
    }

    Column {
        anchors.fill: parent
        anchors.margins: 4

        Item {
            height: parent.height / 2
            width: parent.width

            Rectangle {
                anchors.fill: parent
                anchors.margins: 4
                color: Qt.rgba(0, 0, 0, 0)

                Rectangle {
                    radius: 2
                    width: parent.width * player.health / player.maxHealth
                    height: parent.height
                    color: Qt.rgba(0.3, 0.7, 0.3, 1.0)
                }
            }

            Text {
                anchors.left: parent.left
                anchors.margins: 8
                anchors.verticalCenter: parent.verticalCenter
                text: "HP:"
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                text: player.health + "/" + player.maxHealth
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 8
                anchors.right : parent.right
                text: "+" + player.healthRegen
            }
        }

        Item {
            height: parent.height / 2
            width: parent.width

            Rectangle {
                anchors.fill: parent
                anchors.margins: 4
                color: Qt.rgba(0, 0, 0, 0)

                Rectangle {
                    radius: 2
                    width: parent.width * player.mana / player.maxMana
                    height: parent.height
                    color: Qt.rgba(0.3, 0.3, 0.7, 1.0)
                }
            }

            Text {
                anchors.left: parent.left
                anchors.margins: 8
                anchors.verticalCenter: parent.verticalCenter
                text: "MP:"
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                text: player.mana + "/" + player.maxMana
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.margins: 8
                anchors.right : parent.right
                text: "+" + player.manaRegen
            }
        }
    }
}
