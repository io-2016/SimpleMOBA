import QtQuick 2.2
import Environment 1.0

Image {
    property color textColor: "white"

    id: mainMenu
    source: "qrc:/resources/las.jpeg"

    onVisibleChanged: app.lockedCursor = !visible

    MouseArea {
        anchors.fill: parent
        onWheel: wheel.accepted = true
        hoverEnabled: true
    }

    Text {
        text: "SimpleMOBA"
        color: textColor
        font.pixelSize: 50
        anchors.horizontalCenter: parent.horizontalCenter
        y: mainMenuView.y * 0.5
    }

    MainMenuView {
        id: mainMenuView
        fontSize: 30
        menu: mainMenu
        settingsView: menuSettingsView
        textColor: mainMenu.textColor
    }

    Settings {
        id: menuSettingsView
        anchors.centerIn: parent
        visible: false
        fontSize: 30
        textColor: mainMenu.textColor
        menu: mainMenuView
    }

    Text {
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        text: "version 0.03 " + app.gitVersion
        color: textColor
    }

    Text {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        font.pixelSize: 20

        text: "Filip Czaplicki\nKrzysztof Wojcólewicz\nBartosz Bułkasiewicz\nPaweł Swagner"
        color: textColor
    }
}
