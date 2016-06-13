TEMPLATE = lib
QT += quick multimedia
CONFIG += c++14
CONFIG -= debug_and_release
OBJECTS_DIR = .obj
MOC_DIR = .moc
TARGET = src
GIT_VERSION = $$system(git --work-tree $$PWD describe --always --tags)
DEFINES += GIT_VERSION=\\\"$$GIT_VERSION\\\"
android: QMAKE_CXXFLAGS += -std=c++14

SOURCES += \
    Utility/Window.cpp \
    Entities/Game.cpp \
    Entities/ViewWorld.cpp \
    Entities/World.cpp \
    Utility/Path.cpp \
    Entities/Player.cpp \
    Entities/Enemy.cpp

HEADERS += \
    Entities/Game.hpp \
    Entities/ViewWorld.hpp \
    Entities/World.hpp \
    Utility/Window.hpp \
    Utility/Path.hpp \
    Entities/Player.hpp \
    Entities/Enemy.hpp

unix|win32: LIBS += -L$$OUT_PWD/GameEngine/ -lGameEngine
INCLUDEPATH += $$PWD/GameEngine
DEPENDPATH += $$PWD/GameEngine

unix|win32: LIBS += -L$$OUT_PWD/GameEngine/SceneGraph/ -lSceneGraph
INCLUDEPATH += $$PWD/GameEngine/SceneGraph
DEPENDPATH += $$PWD/GameEngine/SceneGraph

unix|win32: LIBS += -L$$OUT_PWD/GameEngine/Box2D/ -lBox2D
INCLUDEPATH += $$PWD/GameEngine/Box2D
DEPENDPATH += $$PWD/GameEngine/Box2D

RESOURCES += \
    qmlcode.qrc
