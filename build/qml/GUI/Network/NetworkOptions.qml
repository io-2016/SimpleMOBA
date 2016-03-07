import QtQuick 2.2
import QtQuick.Controls 1.1
import Network 1.0

FocusScope {
    width: childrenRect.width
    height: childrenRect.height

    Client {
        id: client
        world: world
        udpPort: 1337
    }

    Row {
        Column {
            Button {
                text: "Connect"
                width: parent.width
                onClicked: clientSettings.focus ^= 1
            }
            Button {
                text: "HostSettings"
                onClicked: hostSettings.focus ^= 1
            }
        }

        FocusScope {
            id: clientSettings
            visible: focus
            width: childrenRect.width
            height: childrenRect.height
            Row {
                Column {
                    id: column

                    Option {
                        id: address
                        name: "Address"
                        text: "127.0.0.1"
                    }

                    Option {
                        id: port
                        name: "Port"
                        text: hostPort.text
                        textField.validator: IntValidator {
                            bottom: 0
                            top: 65536
                        }
                    }

                    Button {
                        text: "Join"
                        width: parent.width
                        onClicked: client.join(address.text, parseInt(port.text))
                    }
                }
            }
        }

        FocusScope {
            id: hostSettings
            visible: focus
            width: childrenRect.width
            height: childrenRect.height
            Row {
                Option {
                    id: hostPort
                    name: "HostPort"
                    text: "1337"
                    textField.validator: IntValidator {
                        bottom: 0
                        top: 65536
                    }
                    textField.onEditingFinished: client.udpPort = parseInt(text)
                }
            }
        }
    }
}
