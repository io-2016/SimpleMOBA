#include "Window.hpp"

#include <QDebug>
#include <QQmlContext>
#include <QQmlEngine>
#include <QTimer>

#ifdef Q_OS_LINUX
#include <QX11Info>
#include <X11/Xlib.h>
#endif

#include "Utility/Factory.hpp"

namespace Utility {

Environment::Environment(Window *view) : QObject(view), m_view(view) {}

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
  if (fullscreen() == enable)
    return;

  if (enable)
    view()->showFullScreen();
  else
    view()->show();

  emit fullscreenChanged();
}

bool Environment::lockedCursor() const { return view()->lockedCursor(); }

void Environment::setLockedCursor(bool e) {
  if (lockedCursor() == e)
    return;
  view()->setLockedCursor(e);
  emit lockedCursorChanged();
}

Window::Window(QWindow *parent)
    : SceneGraph::Window(parent), m_environment(this), m_game(rootItem()),
      m_lockedCursor(false) {
  qmlRegisterUncreatableType<Environment>("Environment", 1, 0, "Environment",
                                          "Uncreatable type!");
  rootContext()->setContextProperty("app", &m_environment);

  World *world = m_game.view()->world();
  MainAction *mainAction = world->mainAction();
  MapEditor *mapEditor = mainAction->mapEditor();
  FileAction *fileAction = mainAction->fileAction();
  SaveMapAction *saveMapAction = fileAction->saveMapAction();
  LoadMapAction *loadMapAction = fileAction->loadMapAction();

  AddBody *addBody = mapEditor->addBody();
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

void Window::setLockedCursor(bool e) {
  if (m_lockedCursor == e)
    return;

  if (e) {
    if (!lockCursor()) {
      QTimer::singleShot(1000, this, &Window::lockCursor);
    } else {
      m_lockedCursor = true;
    }
  } else {
    if (unlockCursor()) {
      m_lockedCursor = false;
    }
  }
}

bool Window::lockCursor() {
#ifdef Q_OS_LINUX
  return XGrabPointer(QX11Info::display(), winId(), true, 0, GrabModeAsync,
                      GrabModeAsync, winId(), None, CurrentTime) == GrabSuccess;
#endif
}

bool Window::unlockCursor() {
#ifdef Q_OS_LINUX
  XUngrabPointer(QX11Info::display(), CurrentTime);
  return true;
#endif
}

void Window::resizeEvent(QResizeEvent *event) {
  SceneGraph::Window::resizeEvent(event);

  QMatrix4x4 matrix;
  matrix.ortho(0, width(), height(), 0, -1, 1);

  setProjection(matrix);

  m_game.setSize(size());
  m_game.resetTransform();
}

} //  namespace Utility
