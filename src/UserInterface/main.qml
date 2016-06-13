import QtQuick 2.2
import QtQuick.Controls 1.1
import "../GameEngine/UserInterface"
import "InGame"
import "."

Item {
    id: root
    width: 1366
    height: 768
    focus: true

    HUDMain {
        id: idHUDMain
        anchors.fill: parent

        onSpellCast: {
            world.castSpell(x, y, id)
        }
    }
    GameEngineConsole {
        id: consoleView
    }

    Rectangle {
        width: 0.05 * parent.width
        height: 0.05 * parent.height
        radius: 5
        color: "green"
        visible: app.system === "android"
        opacity: 0.5

        MultiPointTouchArea {
            anchors.fill: parent
            onPressed: consoleView.consoleEnabled ^= 1
        }
    }

    Menu {
        id: menu
        anchors.fill: parent
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            menu.visible ^= 1
        } if (event.key === Qt.Key_QuoteLeft) {
            mainAction.quit()
            consoleView.consoleEnabled ^= 1
        } else if (event.key === Qt.Key_Comma) {
            game.dump("")
        } else {
            event.accepted = false
        }
    }

    Keys.forwardTo: [
        idHUDMain
    ]

}
