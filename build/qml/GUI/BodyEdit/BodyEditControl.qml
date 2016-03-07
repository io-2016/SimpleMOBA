import QtQuick 2.2
import QtQuick.Controls 1.1

MouseArea {
    property int margin: 5

    id: options
    x: anchors.leftMargin
    anchors.bottomMargin: margin
    anchors.leftMargin: margin
    width: column.width+2*margin
    height: column.height+2*margin
    visible: false
    state: bodyEdit.body ? "enabled" : "disabled"

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
                    to: target.parent.height-target.height-target.anchors.bottomMargin
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
                    from: target.parent.height-target.height-target.anchors.bottomMargin
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

    Connections {
        property int acc: 2

        target: bodyEdit
        onBodyChanged: {
            if (target.body) {
                density.text = target.density.toFixed(acc)
                friction.text = target.friction.toFixed(acc)
                restitution.text = target.restitution.toFixed(acc)
                gravityScale.text = target.gravityScale.toFixed(acc)
                linearDamping.text = target.linearDamping.toFixed(acc)
                angularDamping.text = target.angularDamping.toFixed(acc)

                if (target.dynamic)
                    dynamicRadio.checked = true
                else
                    staticRadio.checked = true
            }
        }
    }

    Column {
        id: column
        anchors.centerIn: parent
        spacing: options.margin

        Label {
            width: bodyType.width
            height: bodyType.height

            Row {
                id: bodyType

                Rectangle {
                    width: 0.5*parent.width
                    height: parent.height
                    color: "transparent"

                    Text {
                        text: "bodyType"
                        anchors.centerIn: parent
                    }
                }

                ExclusiveGroup { id: group }

                RadioButton {
                    id: dynamicRadio
                    text: "Dynamic"
                    exclusiveGroup: group
                    onCheckedChanged: bodyEdit.dynamic = checked
                }

                RadioButton {
                    id: staticRadio
                    text: "Static"
                    exclusiveGroup: group
                }
            }
        }

        Option {
            id: density
            name: "density"
            onTextChanged: bodyEdit.density = parseFloat(text)
        }
        Option {
            id: friction
            name: "friction"
            onTextChanged: bodyEdit.friction = parseFloat(text)
        }
        Option {
            id: restitution
            name: "restitution"
            onTextChanged: bodyEdit.restitution = parseFloat(text)
        }
        Option {
            id: gravityScale
            name: "gravityScale"
            onTextChanged: bodyEdit.gravityScale = parseFloat(text)
        }
        Option {
            id: linearDamping
            name: "linearDamping"
            onTextChanged: bodyEdit.linearDamping = parseFloat(text)
        }
        Option {
            id: angularDamping
            name: "angularDamping"
            onTextChanged: bodyEdit.angularDamping = parseFloat(text)
        }

        Label {
            width: parent.width
            height: applyChangesButton.height

            Button {
                id: applyChangesButton
                anchors.fill: parent

                text: "Apply changes"
                onClicked: bodyEdit.applyChanges()
            }
        }
    }
}
