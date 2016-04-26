import QtQuick 2.2
import QtQuick.Controls 1.2
import ".."

PopUpControl {
    state: fixtureEdit.fixture ? "enabled" : "disabled"
    width: column.width + 2 * margin
    height: column.height + 2 * margin

    Column {
        id: column
        Row {
            id: option

            Rectangle {
                width: 0.3 * parent.width
                height: parent.height
                color: "transparent"

                Text {
                    id: fieldName
                    text: "Texture"
                    anchors.centerIn: parent
                }
            }

            TextField {
                id: text
                text: fixtureEdit.textureSource
                width: 200
            }
        }
        Button {
            width: parent.width
            text: "Apply"
            onClicked: {
                fixtureEdit.textureSource = text.text
                fixtureEdit.apply()
            }
        }
    }
}
