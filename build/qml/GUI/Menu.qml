import QtQuick 2.2

Image {
    id: menu
    source: "qrc:/resources/las.jpeg"

    Text {
        text: "SimpleMOBA"
        color: "white"
        font.pixelSize: 50
        anchors.horizontalCenter: parent.horizontalCenter
        y: rect.y * 0.5
    }

    Rectangle {
        id: rect
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: -column.width * 0.5

        Column {
            id: column
            Text {
                MouseArea {
                    anchors.fill: parent
                    onClicked: menu.visible = false
                }
                text: "Play"
                font.pixelSize: 30
                color: "white"

            }

            Text {
                MouseArea {
                    anchors.fill: parent
                    onClicked: console.log("settings")
                }
                text: "Settings"
                color: "white"
                font.pixelSize: 30
            }

            Text {
                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.quit()
                }
                text: "Quit"
                color: "white"
                font.pixelSize: 30
            }
        }
    }

    Text {
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        text: "version 0.01"
        color: "white"
    }

    Text {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        font.pixelSize: 20

        text: "Filip Czaplicki\nKrzysztof Wojcólewicz\nBartosz Bułkasiewicz\nPaweł Swagner"
        color: "white"
    }
}
