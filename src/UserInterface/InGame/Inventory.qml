import QtQuick 2.0

Item {
    id: idInventoryWrap
    height: idGrid.height + 6
    width: idItemGrid.width + 6

    Rectangle {
        anchors.fill: parent
        anchors.margins: 2
        color: Qt.rgba(0.3, 0.3, 0.3, 1.0)
    }

    Grid {
        id: idItemGrid
        anchors.centerIn: parent
        rows: 2
        columns: 3

        Repeater {
            model: parent.rows * parent.columns

            onWidthChanged: {
                consoleView.log(width)
            }

            delegate: Item {
                height: (idInventoryWrap.height - 6) / 2
                width: height

                Rectangle {
                    anchors.fill: parent
                    anchors.margins: 2
                    radius: 2
                    color: Qt.rgba(0.4, 0.4, 0.4, 1.0)
                }
            }
        }
    }
}
