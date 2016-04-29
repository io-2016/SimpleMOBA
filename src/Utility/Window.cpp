#include "Window.hpp"

#include <QDebug>
#include <QQmlContext>
#include <QQmlEngine>
#include <QTimer>

#include "Entities/Player.hpp"

#include "Utility/Factory.hpp"

namespace Utility {

Environment::Environment(Window *view) : QObject(view), m_view(view) {}

QString Environment::system() const {
  return SceneGraph::Window::systemToString(view()->system()).c_str();
}

bool Environment::fullscreen() const { return view()->fullscreen(); }

void Environment::setFullscreen(bool enable) {
  if (fullscreen() == enable) return;
  view()->setFullScreen(enable);
  emit fullscreenChanged();
}

bool Environment::lockedCursor() const { return view()->lockedCursor(); }

void Environment::setLockedCursor(bool e) {
  if (lockedCursor() == e) return;
  view()->setLockedCursor(e);
  emit lockedCursorChanged();
}

bool Environment::allowLockCursor() const { return view()->allowLockCursor(); }

void Environment::setAllowLockCursor(bool e) {
  if (allowLockCursor() == e) return;
  view()->setAllowLockCursor(e);
  emit allowLockCursorChanged();
}

QString Environment::gitVersion() const { return GIT_VERSION; }

Window::Window(QWindow *parent)
    : SceneGraph::Window(parent), m_environment(this), m_game(rootItem()) {
  rootContext()->setContextProperty("app", &m_environment);
  rootContext()->setContextProperty("world", m_game.view()->world()->object());
  m_game.view()->world()->mainAction()->registerUserInterface(rootContext());

  setSource(QUrl("qrc:/UserInterface/main.qml"));
  setResizeMode(SizeRootObjectToView);

  connect(engine(), &QQmlEngine::quit, this, &QQuickView::close);
  connect(this, &Window::sceneGraphInitialized, this,
          &Window::onSceneGraphInitialized);
  connect(this, &Window::sceneGraphInvalidated, this,
          &Window::onSceneGraphInvalidated);
}

Window::~Window() {}

void Window::onSceneGraphInitialized() {}

void Window::onSceneGraphInvalidated() {}

void Window::resizeEvent(QResizeEvent *event) {
  SceneGraph::Window::resizeEvent(event);

  QMatrix4x4 matrix;
  matrix.ortho(0, width(), height(), 0, -1, 1);

  setProjection(matrix);

  m_game.setSize(size());
  m_game.resetTransform();
}

}  //  namespace Utility
