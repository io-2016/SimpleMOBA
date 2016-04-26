import QtQuick 2.2
import QtQuick.Controls 1.1
import ".."

PopUpControl {
    property int margin: 5

    id: options
    width: column.width + 2 * margin
    height: column.height + 2 * margin
    state: bodyEdit.body ? "enabled" : "disabled"

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
