import QtQuick 2.2
import QtQuick.Controls 1.1
import Environment 1.0

Row {
    id: mainMenu

    Column {
        Button {
            text: "Quit"
            onClicked: Qt.quit()
        }

        Button {
            text: "MapEditor"
            onClicked: mapEditor.toggleEnabled()
        }

        Button {
            text: "File"
            onClicked: fileAction.toggleEnabled()
        }

        Button {
            text: "Pause"
            onClicked: world.paused ^= 1
        }

        Button {
            visible: app.system !== Environment.Android
            text: "Toggle fullscreen"
            onClicked: app.fullscreen ^= 1
        }

        Component.onCompleted: {
            var max = 0, i
            for (i = 0; i < children.length; i++)
                if (children[i].visible)
                    max = Math.max(children[i].width, max)
            for (i = 0; i < children.length; i++)
                if (children[i].visible)
                    children[i].width = max
        }
    }

    Column {
        visible: fileAction.enabled

        SaveMap { }
        LoadMap { }
    }

    Row {
        visible: mapEditor.enabled

        Column {
            Button {
                width: parent.width
                text: "AddChain"
                onClicked: addChain.toggleEnabled()
            }

            Button {
                text: "DeleteItem"
                onClicked: deleteItem.toggleEnabled()
            }

            Button {
                width: parent.width
                text: "GrabItem"
                onClicked: grabItem.toggleEnabled()
            }

            Button {
                width: parent.width
                text: "BodyEdit"
                onClicked: bodyEdit.toggleEnabled()
            }

            Button {
                width: parent.width
                text: "FixtureEdit"
                onClicked: fixtureEdit.toggleEnabled()
            }

            Button {
                width: parent.width
                text: "AddBody"
                onClicked: addBody.toggleEnabled()
            }
        }

        Column {
            visible: addBody.enabled

            Button {
                width: parent.width
                text: "AddPolygon"
                onClicked: addPolygon.toggleEnabled()
            }

            Button {
                width: parent.width
                text: "AddCircle"
                onClicked: addCircle.toggleEnabled()
            }

            Button {
                text: "AddRectangle"
                onClicked: addRectangle.toggleEnabled()
            }

            Button {
                width: parent.width
                text: "FinishBody"
                onClicked: addBody.finished()
            }
        }

        Column {
            id: polygonOptions
            visible: addPolygon.enabled

            Button {
                text: "Finish"
                onClicked: addPolygon.finished()
            }
        }
    }
}
