import QtQuick 2.0

Item {
    Rectangle {
        anchors.fill: parent
        anchors.margins: 4
        color: Qt.rgba(0.3, 0.3, 0.3, 1.0)
        radius: 4

        Item {
            id: idShopLabel
            height: idLabelText.height + 4
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 4

            Rectangle {
                anchors.fill: parent
                color: Qt.rgba(1.0, 1.0, 1.0, 1.0)
            }

            Text {
               id: idLabelText
               anchors.centerIn: parent
               text: "SHOP"
            }
        }

        Item {
            id: idItemGrid
            anchors.bottom: parent.bottom
            anchors.top: idShopLabel.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            clip: true

            property var items: [
                "/spell_icons/resources/enchant-acid-1.png",
                "/spell_icons/resources/enchant-acid-2.png",
                "/spell_icons/resources/enchant-acid-3.png",
                "/spell_icons/resources/enchant-blue-1.png",
                "/spell_icons/resources/enchant-blue-2.png",
                "/spell_icons/resources/enchant-blue-3.png",
                "/spell_icons/resources/enchant-eerie-1.png",
                "/spell_icons/resources/enchant-eerie-2.png",
                "/spell_icons/resources/enchant-eerie-3.png",
                "/spell_icons/resources/enchant-jade-1.png",
                "/spell_icons/resources/enchant-jade-2.png",
                "/spell_icons/resources/enchant-jade-3.png",
                "/spell_icons/resources/enchant-magenta-1.png",
                "/spell_icons/resources/enchant-magenta-2.png",
                "/spell_icons/resources/enchant-magenta-3.png",
                "/spell_icons/resources/enchant-orange-1.png",
                "/spell_icons/resources/enchant-orange-2.png",
                "/spell_icons/resources/enchant-orange-3.png",
                "/spell_icons/resources/enchant-red-1.png",
                "/spell_icons/resources/enchant-red-2.png",
                "/spell_icons/resources/enchant-red-3.png",
                "/spell_icons/resources/enchant-royal-1.png",
                "/spell_icons/resources/enchant-royal-2.png",
                "/spell_icons/resources/enchant-royal-3.png",
                "/spell_icons/resources/enchant-sky-1.png",
                "/spell_icons/resources/enchant-sky-2.png",
                "/spell_icons/resources/enchant-sky-3.png",
                "/spell_icons/resources/evil-eye-eerie-1.png",
                "/spell_icons/resources/evil-eye-eerie-2.png",
                "/spell_icons/resources/evil-eye-eerie-3.png",
                "/spell_icons/resources/evil-eye-red-1.png",
                "/spell_icons/resources/evil-eye-red-2.png",
                "/spell_icons/resources/evil-eye-red-3.png",
                "/spell_icons/resources/fireball-acid-1.png",
                "/spell_icons/resources/fireball-acid-2.png",
                "/spell_icons/resources/fireball-acid-3.png",
                "/spell_icons/resources/fireball-eerie-1.png",
                "/spell_icons/resources/fireball-eerie-2.png",
                "/spell_icons/resources/fireball-eerie-3.png",
                "/spell_icons/resources/fireball-red-1.png",
                "/spell_icons/resources/fireball-red-2.png",
                "/spell_icons/resources/fireball-red-3.png",
                "/spell_icons/resources/fireball-sky-1.png",
                "/spell_icons/resources/fireball-sky-2.png",
                "/spell_icons/resources/fireball-sky-3.png",
                "/spell_icons/resources/heal-jade-1.png",
                "/spell_icons/resources/heal-jade-2.png",
                "/spell_icons/resources/heal-jade-3.png",
                "/spell_icons/resources/heal-royal-1.png",
                "/spell_icons/resources/heal-royal-2.png",
                "/spell_icons/resources/heal-royal-3.png",
                "/spell_icons/resources/heal-sky-1.png",
                "/spell_icons/resources/heal-sky-2.png",
                "/spell_icons/resources/heal-sky-3.png",
                "/spell_icons/resources/protect-acid-1.png",
                "/spell_icons/resources/protect-acid-2.png",
                "/spell_icons/resources/protect-acid-3.png",
                "/spell_icons/resources/protect-blue-1.png",
                "/spell_icons/resources/protect-blue-2.png",
                "/spell_icons/resources/protect-blue-3.png",
                "/spell_icons/resources/protect-eerie-1.png",
                "/spell_icons/resources/protect-eerie-2.png",
                "/spell_icons/resources/protect-eerie-3.png",
                "/spell_icons/resources/protect-jade-1.png",
                "/spell_icons/resources/protect-jade-2.png",
                "/spell_icons/resources/protect-jade-3.png",
                "/spell_icons/resources/protect-magenta-1.png",
                "/spell_icons/resources/protect-magenta-2.png",
                "/spell_icons/resources/protect-magenta-3.png",
                "/spell_icons/resources/protect-orange-1.png",
                "/spell_icons/resources/protect-orange-2.png",
                "/spell_icons/resources/protect-orange-3.png",
                "/spell_icons/resources/protect-red-1.png",
                "/spell_icons/resources/protect-red-2.png",
                "/spell_icons/resources/protect-red-3.png",
                "/spell_icons/resources/protect-royal-1.png",
                "/spell_icons/resources/protect-royal-2.png",
                "/spell_icons/resources/protect-royal-3.png",
                "/spell_icons/resources/protect-sky-1.png",
                "/spell_icons/resources/protect-sky-2.png",
                "/spell_icons/resources/protect-sky-3.png",
            ]

            GridView {
                anchors.fill: parent

                model: 81
                cellWidth: parent.width / 5

                delegate: Item {
                    width: idItemGrid.width / 5
                    height: width + idCost.height

                    Rectangle {
                        id: idItemBackground
                        anchors.fill: parent
                        anchors.margins: 4
                    }

                    Image {
                        id: idItemIcon
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.top: parent.top
                        anchors.bottom: idCost.top

                        source: idItemGrid.items[index]
                    }

                    Text {
                        id: idCost
                        text: "300"
                        anchors.bottom: parent.bottom
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.margins: 8
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log("Clicked", index)
                        }
                    }
                }
            }
        }
    }
}
