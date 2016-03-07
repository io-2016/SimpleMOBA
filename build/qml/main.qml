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

    BodyEditControl { }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape)
            Qt.quit();
        else if (event.key === Qt.Key_F1)
            mainMenu.toggle()
        else if (event.key === Qt.Key_Comma)
            game.dump("")
    }

}
