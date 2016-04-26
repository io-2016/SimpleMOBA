import QtQuick 2.2
import QtQuick.Controls 1.1

Item {
    property alias name: fieldName.text
    property alias text: text.text

    width: option.width
    height: option.height

    Row {
        id: option
        anchors.centerIn: parent

        Rectangle {
            width: 0.5 * parent.width
            height: parent.height
            color: "transparent"

            Text {
                id: fieldName
                anchors.centerIn: parent
            }
        }

        TextField {
            id: text
            validator: DoubleValidator { locale: "C" }
        }
    }
}
