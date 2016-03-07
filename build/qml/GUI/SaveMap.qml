import QtQuick 2.2
import QtQuick.Controls 1.0

FocusScope {
    width: childrenRect.width
    height: childrenRect.height

    Row {
        Button {
            text: "SaveMap"
            onClicked: saveMap.toggleEnabled()
        }

        Row {
            enabled: visible
            visible: saveMap.enabled

            Column {
                Row {
                    Button {
                        text: "Save to file:"
                        onClicked: saveMap.dump(fileName.text);
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
