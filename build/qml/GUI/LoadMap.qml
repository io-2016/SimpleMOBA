import QtQuick 2.2
import QtQuick.Controls 1.1

FocusScope {
    width: childrenRect.width
    height: childrenRect.height

    Row {
        Button {
            text: "LoadMap"
            onClicked: loadMap.toggleEnabled()
        }

        Row {
            enabled: visible
            visible: loadMap.enabled

            Column {
                Row {
                    Button {
                        text: "Load from file:"
                        onClicked: loadMap.load(fileName.text);
                    }

                    TextField {
                        id: fileName
                        text: "map00.json"
                    }
                }
            }
        }
    }
}
