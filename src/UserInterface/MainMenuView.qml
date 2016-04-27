import QtQuick 2.0

Rectangle {
    property Item menu
    property Settings settingsView
    property color textColor
    property int fontSize: 30

    id: mainView
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
            font.pixelSize: fontSize
            color: textColor

        }

        Text {
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    mainView.visible = false
                    settingsView.visible = true
                }
            }
            text: "Settings"
            color: textColor
            font.pixelSize: fontSize
        }

        Text {
            MouseArea {
                anchors.fill: parent
                onClicked: Qt.quit()
            }
            text: "Quit"
            color: textColor
            font.pixelSize: fontSize
        }
    }
}
