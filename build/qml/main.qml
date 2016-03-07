import QtQuick 2.2
import QtQuick.Controls 1.1
import Environment 1.0
import "GUI"
import "GUI/BodyEdit"

Item {
    id: root
    width: 800
    height: 600
    focus: true

    GameOver { }
    MenuDisplay { id: mainMenu }
    Data { id: debug }
    HUD { id: hud }
    AndroidUI {
        visible: app.system === Environment.Android
        opacity: 0.5
    }

    BodyEditControl { }

    /*Text {
        anchors.centerIn: parent
        text: "PAUSED"
        visible: world.paused
        scale: 5
        color: "blue"
    }*/

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape)
            Qt.quit();
        else if (event.key === Qt.Key_F1)
            mainMenu.toggle()
        else if (event.key === Qt.Key_Comma)
            game.dump("")
    }

}
