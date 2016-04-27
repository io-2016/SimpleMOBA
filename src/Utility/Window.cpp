#include "Window.hpp"

#include <QDebug>
#include <QQmlContext>
#include <QQmlEngine>
#include <QTimer>

#include "Entities/Player.hpp"

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
  if (fullscreen() == enable) return;

  if (enable)
    view()->showFullScreen();
  else
    view()->show();

  emit fullscreenChanged();
}

bool Environment::lockedCursor() const { return view()->lockedCursor(); }

void Environment::setLockedCursor(bool e) {
  if (lockedCursor() == e) return;
  view()->setLockedCursor(e);
  emit lockedCursorChanged();
}

QString Environment::gitVersion() const { return GIT_VERSION; }

Window::Window(QWindow *parent)
    : SceneGraph::Window(parent),
      m_environment(this),
      m_game(rootItem()) {
  qmlRegisterUncreatableType<Environment>("Environment", 1, 0, "Environment",
                                          "Uncreatable type!");
  rootContext()->setContextProperty("app", &m_environment);
  rootContext()->setContextProperty("world", m_game.view()->world()->object());
  m_game.view()->world()->mainAction()->registerUserInterface(rootContext());

  setSource(QUrl("qrc:/UserInterface/main.qml"));
  setResizeMode(SizeRootObjectToView);

  connect(engine(), &QQmlEngine::quit, this, &QQuickView::close);
}

Window::~Window() { }

void Window::resizeEvent(QResizeEvent *event) {
  SceneGraph::Window::resizeEvent(event);

  QMatrix4x4 matrix;
  matrix.ortho(0, width(), height(), 0, -1, 1);

  setProjection(matrix);

  m_game.setSize(size());
  m_game.resetTransform();

}

}  //  namespace Utility
