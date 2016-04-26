import QtQuick 2.2
import QtQuick.Controls 1.1

MouseArea {
    property int margin: 5

    id: options
    x: anchors.leftMargin
    anchors.bottomMargin: margin
    anchors.leftMargin: margin
    visible: false

    TextField {
        anchors.fill: parent
    }

    states: [
        State {
            name: "enabled"
            AnchorChanges {
                target: options
                anchors.bottom: options.parent.bottom
                anchors.left: options.parent.left
            }
        },
        State {
            name: "disabled"
            AnchorChanges {
                target: options
                anchors.bottom: undefined
                anchors.left: undefined
            }
        }
    ]

    transitions: [
        Transition {
            from: "disabled"
            to: "enabled"

            SequentialAnimation {
                PropertyAction {
                    target: options
                    property: "visible"
                    value: true
                }
                NumberAnimation {
                    target: options
                    property: "y"
                    duration: 400
                    from: target.parent.height
                    to: target.parent.height-target.height - target.anchors.bottomMargin
                    easing.type: Easing.OutQuad
                }
            }
        },

        Transition {
            from: "enabled"
            to: "disabled"

            SequentialAnimation {
                NumberAnimation {
                    target: options
                    property: "y"
                    duration: 400
                    from: target.parent.height-target.height - target.anchors.bottomMargin
                    to: target.parent.height
                    easing.type: Easing.InQuad
                }
                PropertyAction {
                    target: options
                    property: "visible"
                    value: false
                }
            }
        }
    ]

    Rectangle {
        anchors.fill: parent
        radius: 10
    }
}
