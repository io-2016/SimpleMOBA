TEMPLATE = lib

QT += quick
CONFIG += c++11

OBJECTS_DIR = .obj
MOC_DIR = .moc

QMAKE_CXXFLAGS += -fno-strict-aliasing
LIBS += -L$$OUT_PWD/../build -lSceneGraph -lGameEngine

INCLUDEPATH += $$PWD/../deps/ $$PWD/../deps/GameEngine/

DESTDIR = $$OUT_PWD/../build
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

