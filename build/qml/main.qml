import QtQuick 2.2
import QtQuick.Controls 1.1
import Environment 1.0
import "GUI"
import "GUI/BodyEdit"
import "GUI/FixtureEdit"
import "GUI/InGame"

Item {
    id: root
    width: 1366
    height: 768
    focus: true

    HUDMain { anchors.fill: parent }
    GameOver { }
    ConsoleView { id: consoleView }
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
            onPressed: consoleView.toggle()
        }
    }

    BodyEditControl { }
    FixtureEditControl { }

    Menu {
        id: menu
        anchors.fill: parent
    }

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape)
            menu.visible ^= 1
        else if (event.key === Qt.Key_QuoteLeft)
            consoleView.toggle()
        else if (event.key === Qt.Key_Comma)
            game.dump("")
    }

}
