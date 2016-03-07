import QtQuick 2.2
import QtQuick.Controls 1.1

Item {
    anchors.fill: parent

    ProgressBar {
        id: healthbar
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        minimumValue: 0
        maximumValue: 100
        value: world.playerHealth
        Text {
            anchors.centerIn: parent
            text: "Health: "+healthbar.value+"%"
        }
    }

    ProgressBar {
        id: ammobar
        anchors.bottom: healthbar.top
        anchors.right: parent.right
        visible: world.equippedWeapon
        value: visible ? world.bulletCount : 0
        minimumValue: 0
        maximumValue: 15
        Text {
            anchors.centerIn: parent
            text: "Ammo: "+ammobar.value
        }
    }
}
