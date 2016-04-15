TEMPLATE = lib
QT += quick
CONFIG += c++11
CONFIG -= debug_and_release
OBJECTS_DIR = .obj
MOC_DIR = .moc
TARGET = src

SOURCES += \
    Utility/Window.cpp \
    Entities/Game.cpp \
    Entities/ViewWorld.cpp \
    Entities/World.cpp \

HEADERS += \
    Entities/Game.hpp \
    Entities/ViewWorld.hpp \
    Entities/World.hpp \
    Utility/Window.hpp

unix|win32: LIBS += -L$$OUT_PWD/GameEngine/ -lGameEngine
INCLUDEPATH += $$PWD/GameEngine
DEPENDPATH += $$PWD/GameEngine

unix|win32: LIBS += -L$$OUT_PWD/GameEngine/SceneGraph/ -lSceneGraph
INCLUDEPATH += $$PWD/GameEngine/SceneGraph
DEPENDPATH += $$PWD/GameEngine/SceneGraph

unix|win32: LIBS += -L$$OUT_PWD/GameEngine/Box2D/ -lBox2D
INCLUDEPATH += $$PWD/GameEngine/Box2D
DEPENDPATH += $$PWD/GameEngine/Box2D
