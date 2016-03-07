import QtQuick 2.2
import QtQuick.Controls 1.1

Item {
    property alias name:  text.text
    property alias text: field.text
    property alias textField: field

    width: childrenRect.width
    height: childrenRect.height

    Row {
        id: option
        Rectangle {
            width: 0.5*parent.width
            height: parent.height
            color: "transparent"
            Text {
                id: text
                anchors.centerIn: parent
            }
        }
        TextField {
            id: field
        }
    }
}
