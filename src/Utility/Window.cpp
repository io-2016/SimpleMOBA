#include "Window.hpp"

#include <QQmlEngine>
#include <QQmlContext>
#include <QDebug>

#include "Entities/Player.hpp"
#include "Entities/Pony.hpp"
#include "Entities/Gun.hpp"
#include "Entities/Deagle.hpp"
#include "Entities/Enemy.hpp"
#include "Entities/Chain.hpp"

#include "QBox2D/QBody.hpp"
#include "QBox2D/Fixture/Box2DBox.hpp"
#include "QBox2D/Fixture/Box2DChain.hpp"
#include "QBox2D/Fixture/Box2DCircle.hpp"
#include "QBox2D/Fixture/Box2DEdge.hpp"
#include "QBox2D/Fixture/Box2DPolygon.hpp"

#include "Network/Server.hpp"
#include "Network/Client.hpp"

#include "Utility/Factory.hpp"

Environment::Environment(QQuickView* view) : QObject(view), m_view(view) {}

Environment::System Environment::system() const {
#if defined Q_OS_ANDROID
  return System::Android;
#elif defined Q_OS_UNIX
  return System::Unix;
#elif defined Q_OS_WIN32
  return System::Win32;
#else
  return System::Unknown;
#endif
}

bool Environment::fullscreen() const {
  return view()->visibility() == QWindow::FullScreen;
}

void Environment::setFullscreen(bool enable) {
  if (fullscreen() == enable) return;

  if (enable)
    view()->showFullScreen();
  else
    view()->show();

  emit fullscreenChanged();
}

Window::Window(QWindow* parent)
    : SceneGraph::Window(parent),
      m_game(rootItem()),
      m_environment(this) {
  rootContext()->setContextProperty("app", &m_environment);

  World* world = m_game.view()->world();
  MainAction* mainAction = world->mainAction();
  MapEditor* mapEditor = mainAction->mapEditor();
  FileAction* fileAction = mainAction->fileAction();
  SaveMapAction* saveMapAction = fileAction->saveMapAction();
  LoadMapAction* loadMapAction = fileAction->loadMapAction();

  AddBody* addBody = mapEditor->addBody();
  rootContext()->setContextProperty("world", world->object());

  rootContext()->setContextProperty("mapEditor", mapEditor->object());
  rootContext()->setContextProperty("fileAction", fileAction->object());

  rootContext()->setContextProperty("saveMap", saveMapAction->object());
  rootContext()->setContextProperty("loadMap", loadMapAction->object());

  rootContext()->setContextProperty("addBody", addBody->object());
  rootContext()->setContextProperty("addPolygon",
                                    addBody->addPolygon()->object());
  rootContext()->setContextProperty("addRectangle",
                                    addBody->addRectangle()->object());
  rootContext()->setContextProperty("addCircle",
                                    addBody->addCircle()->object());

  rootContext()->setContextProperty("grabItem",
                                    mapEditor->grabItem()->object());
  rootContext()->setContextProperty("deleteItem",
                                    mapEditor->deleteItem()->object());
  rootContext()->setContextProperty("bodyEdit",
                                    mapEditor->bodyEdit()->object());
  rootContext()->setContextProperty("addChain",
                                    mapEditor->addChain()->object());

  setSource(QUrl("qrc:/qml/main.qml"));
  setResizeMode(SizeRootObjectToView);

  connect(engine(), &QQmlEngine::quit, this, &QQuickView::close);
}

void Window::resizeEvent(QResizeEvent* event) {
  SceneGraph::Window::resizeEvent(event);

  QMatrix4x4 matrix;
  matrix.ortho(0, width(), height(), 0, -1, 1);

  setProjection(matrix);

  m_game.setSize(size());
  //m_game.resetTransform();
  //m_game.rotate(45, 0, 0, 1);
  //m_game.translate(-300, -300);
}

void Window::registerTypes() {
  qmlRegisterUncreatableType<Environment>("Environment", 1, 0, "Environment",
                                          "Uncreatable type!");
  Utility::registerType<Box2DBox>("Box2DBox");
  Utility::registerType<Box2DChain>("Box2DChain");
  Utility::registerType<Box2DCircle>("Box2DCircle");
  Utility::registerType<Box2DEdge>("Box2DEdge");
  Utility::registerType<Box2DPolygon>("Box2DPolygon");

  Utility::registerType<QBody>("QBody");
  Utility::registerType<Gun>("Gun");
  Utility::registerType<Deagle>("Deagle");
  Utility::registerType<Enemy>("Enemy");
  Utility::registerType<Chain>("Chain");
  Utility::registerType<Player>("Player");
}
