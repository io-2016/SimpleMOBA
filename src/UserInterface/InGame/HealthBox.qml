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
                radius: 2
                color: Qt.rgba(0.3, 0.7, 0.3, 1.0)
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
                text: "100/100"
            }
        }

        Item {
            height: parent.height / 2
            width: parent.width

            Rectangle {
                anchors.fill: parent
                anchors.margins: 4
                radius: 2
                color: Qt.rgba(0.3, 0.3, 0.7, 1.0)
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
                text: "100/100"
            }
        }
    }
}
