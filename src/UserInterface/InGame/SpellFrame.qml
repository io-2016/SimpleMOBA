import QtQuick 2.0

Item {
    id: idContainer
    property string iconPath
    property int spellId
    property bool selected: false

    signal cast

    Item {
        anchors.fill: parent
        anchors.margins: 4

        Image {
            anchors.fill: parent
            source: idContainer.iconPath
        }

        Rectangle {
            id: idSpellTint
            anchors.fill: parent
            color: Qt.rgba(1.0, 1.0, 1.0, 0.0)
            border.color: Qt.rgba(0.3, 1.0, 0.4, 1.0)
            border.width: selected ? 3 : 0
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                idSpellTint.color = Qt.rgba(1.0, 1.0, 1.0, 0.3)
            }

            onExited: {
                idSpellTint.color = Qt.rgba(1.0, 1.0, 1.0, 0.0)
            }

            onClicked: {
                idContainer.cast()
            }
        }
    }
    }
