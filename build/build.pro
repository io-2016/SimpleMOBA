TEMPLATE = app

QT += quick widgets
CONFIG += c++11
QMAKE_CXXFLAGS += -fno-strict-aliasing
OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .rcc
LIBS += -L$$OUT_PWD/../build -lsrc -lBox2D -lSceneGraph -lGameEngine -Wl,-R.
INCLUDEPATH += ../src ../deps ../deps/GameEngine
DESTDIR = $$OUT_PWD/../build
TARGET = SimpleMOBA

SOURCES += main.cpp

RESOURCES += \
    resources.qrc \
    qmlcode.qrc \
    json.qrc

OTHER_FILES += \
    android/AndroidManifest.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
