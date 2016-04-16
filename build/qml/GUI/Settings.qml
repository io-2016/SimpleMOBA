import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Item {
    property Item menu
    property int fontSize
    property color textColor

    id: settings

    anchors.centerIn: parent
    anchors.horizontalCenterOffset: -column.width * 0.5

    Text {
        font.pixelSize: 1.2 * fontSize
        text: "Settings"
        color: textColor
        anchors.bottom: column.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: 0.5 * column.width
    }

    Column {
        id: column

        Row {
            Text {
                font.pixelSize: fontSize
                text: "Login: "
                color: textColor
            }
            TextEdit {
                width: 100
                font.pixelSize: fontSize
                color: textColor
            }
        }

        Row {
            z: 1
            Text {
                text: "Color: "
                font.pixelSize: fontSize
                color: textColor
            }

            ColorComboBox {
                id: colorSelection
                anchors.verticalCenter: parent.verticalCenter
                width: 100
                colors: [ "green", "yellow", "blue", "orange" ]
            }
        }

        Text {
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    settings.visible = false
                    menu.visible = true
                    menu.forceActiveFocus()
                }
            }

            anchors.horizontalCenter: parent.horizontalCenter
            text: "Back"
            font.pixelSize: fontSize
            color: textColor
        }

    }
}
