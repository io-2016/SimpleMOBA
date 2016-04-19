import QtQuick 2.0

Item {
    Rectangle {
        anchors.fill: parent
        anchors.margins: 2
        radius: 2
        color: Qt.rgba(0.3, 0.3, 0.3, 0.5)
        border.width: 1
    }

    Text {
        anchors.centerIn: parent
        text: "Minimap"
    }
}
