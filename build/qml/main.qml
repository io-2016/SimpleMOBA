import QtQuick 2.2
import QtQuick.Controls 1.1
import Environment 1.0
import "GUI"
import "GUI/BodyEdit"
import "GUI/Ingame-GUI"

Item {
    id: root
    width: 800
    height: 600
    focus: true

    HUDMain {
        anchors.fill: parent
    }

    GameOver { }
    MenuDisplay { id: mainMenu }
    Data { id: debug }

    Rectangle {
        width: 0.05 * parent.width
        height: 0.05 * parent.height
        radius: 5
        color: "green"
        visible: app.system === Environment.Android
        opacity: 0.5

        MultiPointTouchArea {
            anchors.fill: parent
            onPressed: mainMenu.toggle()
        }
    }

    BodyEditControl { }

    Menu {
        id: menu
        anchors.fill: parent
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape)
            menu.visible ^= 1
        else if (event.key === Qt.Key_QuoteLeft)
            mainMenu.toggle()
        else if (event.key === Qt.Key_Comma)
            game.dump("")
    }

}
