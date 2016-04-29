#ifndef WINDOW_HPP
#define WINDOW_HPP
#include "Entities/Game.hpp"
#include "SceneGraph/Window.hpp"

namespace Utility {

class Window;

class Environment : public QObject {
 private:
  Q_OBJECT

  Q_PROPERTY(QString system READ system CONSTANT)
  Q_PROPERTY(bool fullscreen READ fullscreen WRITE setFullscreen NOTIFY
                 fullscreenChanged)
  Q_PROPERTY(bool lockedCursor READ lockedCursor WRITE setLockedCursor NOTIFY
                 lockedCursorChanged)
  Q_PROPERTY(bool allowLockCursor READ allowLockCursor WRITE setAllowLockCursor
                 NOTIFY allowLockCursorChanged)
  Q_PROPERTY(QString gitVersion READ gitVersion CONSTANT)

  Window *m_view;

 public:
  Environment(Window *view);

  inline Window *view() const { return m_view; }

  QString system() const;

  bool fullscreen() const;
  void setFullscreen(bool);

  bool lockedCursor() const;
  void setLockedCursor(bool);

  bool allowLockCursor() const;
  void setAllowLockCursor(bool);

  QString gitVersion() const;

 signals:
  void fullscreenChanged();
  void lockedCursorChanged();
  void allowLockCursorChanged();
};

class Window : public SceneGraph::Window {
 private:
  Environment m_environment;
  Game m_game;

  void onSceneGraphInitialized();
  void onSceneGraphInvalidated();

 protected:
  void resizeEvent(QResizeEvent *);

 public:
  Window(QWindow * = nullptr);
  ~Window();

  inline const Game *game() const { return &m_game; }
  inline Game *game() { return &m_game; }
};

}  //  namespace Utility

#endif  // WINDOW_HPP
