import QtQuick 2.0

Item {
    Rectangle {
        anchors.fill: parent
        anchors.margins: 2
        radius: 2
        color: Qt.rgba(0.3, 0.3, 0.3, 0.5)
        border.width: 1
    }

    Image {
        source: "qrc:/resources/dota_minimap.png"
        fillMode: Image.PreserveAspectFit
        width: parent.width
        height: parent.height
    }

    Rectangle {
        anchors.centerIn: parent
        color: app.playerIndicatorColor
        width: parent.width * 0.05
        height: parent.height * 0.05
        radius: width * 0.5
    }
}
