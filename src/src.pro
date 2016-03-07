TEMPLATE = lib

QT += quick network
CONFIG += c++11

OBJECTS_DIR = .obj
MOC_DIR = .moc

QMAKE_CXXFLAGS += -fno-strict-aliasing

LIBS += -L$$OUT_PWD/../build -lBox2D -lSceneGraph

INCLUDEPATH += $$PWD/../deps/

DESTDIR = $$OUT_PWD/../build
TARGET = src

SOURCES += \
    Actions/AddBody.cpp \
    Actions/DeleteItem.cpp \
    Actions/MapEditor.cpp \
    Actions/AddChain.cpp \
    Actions/Action.cpp \
    Actions/AddCircle.cpp \ 
    Actions/AddPolygon.cpp \
    Actions/AddRectangle.cpp \
    Actions/GrabItem.cpp \
    Geometry/Triangle.cpp \
    Geometry/Edge.cpp \
    Geometry/Circle.cpp \
    Geometry/Triangulate.cpp \
    Geometry/Vector2d.cpp \
    Geometry/Functions.cpp \
    QBox2D/QFixture.cpp \
    QBox2D/QBody.cpp \
    QBox2D/QWorld.cpp \
    Actions/BodyEdit.cpp \
    Lighting/DynamicLight.cpp \
    Lighting/LightSystem.cpp \
    Lighting/Light.cpp \
    Utility/Window.cpp \
    Lighting/EnlightedItems.cpp \
    Lighting/LightMaterial.cpp \
    Utility/Utility.cpp \
    Lighting/StaticLight.cpp \
    Lighting/LightBlender.cpp \
    Graphics/TexturedRectangle.cpp \
    Graphics/TexturedConvexPolygon.cpp \
    Graphics/TexturedPolygon.cpp \
    Graphics/TexturedItem.cpp \
    Utility/DisplayItem.cpp \
    QBox2D/Fixture/Fixture.cpp \
    QBox2D/Fixture/Box2DBox.cpp \
    QBox2D/Fixture/Box2DPolygon.cpp \
    QBox2D/Fixture/Box2DCircle.cpp \
    QBox2D/Fixture/Box2DEdge.cpp \
    QBox2D/Fixture/Box2DChain.cpp \
    Graphics/SpriteSequence.cpp \
    QBox2D/QJoint.cpp \
    Utility/BaseItem.cpp \
    Graphics/Primitives.cpp \
    Actions/SubAction.cpp \
    Actions/AddFixture.cpp \
    Graphics/ParticleSystem.cpp \
    Actions/FileAction.cpp \
    Actions/MainAction.cpp \
    QBox2D/QChain.cpp \
    Entities/Game.cpp \
    Entities/ViewWorld.cpp \
    Entities/World.cpp

HEADERS += \
    Actions/Action.hpp \
    Actions/GrabItem.hpp \
    Actions/AddBody.hpp \
    Actions/DeleteItem.hpp \
    Actions/AddPolygon.hpp \
    Actions/AddCircle.hpp \
    Actions/AddRectangle.hpp \
    Actions/AddChain.hpp \
    Actions/MapEditor.hpp \
    Geometry/Functions.hpp \
    Geometry/Circle.hpp \
    Geometry/Triangle.hpp \
    Geometry/Triangulate.hpp \
    Geometry/Edge.hpp \
    Geometry/Vector2d.hpp \
    QBox2D/QFixture.hpp \
    QBox2D/QWorld.hpp \
    QBox2D/QBody.hpp \
    Actions/BodyEdit.hpp \
    Lighting/DynamicLight.hpp \
    Lighting/LightSystem.hpp \
    Lighting/Light.hpp \
    Utility/Window.hpp \
    Lighting/EnlightedItems.hpp \
    Lighting/LightMaterial.hpp \
    Utility/Utility.hpp \
    Lighting/StaticLight.hpp \
    Lighting/LightBlender.hpp \
    Graphics/TexturedRectangle.hpp \
    Graphics/TexturedConvexPolygon.hpp \
    Graphics/TexturedPolygon.hpp \
    Graphics/TexturedItem.hpp \
    Utility/DisplayItem.hpp \
    QBox2D/Fixture/Fixture.hpp \
    QBox2D/Fixture/Box2DBox.hpp \
    QBox2D/Fixture/Box2DPolygon.hpp \
    QBox2D/Fixture/Box2DCircle.hpp \
    QBox2D/Fixture/Box2DEdge.hpp \
    QBox2D/Fixture/Box2DChain.hpp \
    Graphics/SpriteSequence.hpp \
    QBox2D/QJoint.hpp \
    Utility/Factory.hpp \
    Utility/Pool.hpp \
    Utility/List.hpp \
    Utility/BaseItem.hpp \
    Graphics/Primitives.hpp \
    Actions/SubAction.hpp \
    Actions/AddFixture.hpp \
    Graphics/ParticleSystem.hpp \
    Actions/FileAction.hpp \
    Actions/MainAction.hpp \
    QBox2D/QChain.hpp \
    Entities/Game.hpp \
    Entities/ViewWorld.hpp \
    Entities/World.hpp

