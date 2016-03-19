#ifndef WINDOW_HPP
#define WINDOW_HPP
#include "SceneGraph/Window.hpp"
#include "Entities/Game.hpp"

class Environment : public QObject {
 private:
  Q_OBJECT

  Q_PROPERTY(System system READ system CONSTANT)
  Q_PROPERTY(bool fullscreen READ fullscreen WRITE setFullscreen NOTIFY
                 fullscreenChanged)
  Q_ENUMS(System)

  QQuickView* m_view;

 public:
  enum class System { Android, Unix, Win32, Unknown };

  Environment(QQuickView* view);

  inline QQuickView* view() const { return m_view; }

  System system() const;

  bool fullscreen() const;
  void setFullscreen(bool);

 signals:
  void fullscreenChanged();
};

class Window : public SceneGraph::Window {
 private:
  Environment m_environment;
  Game m_game;

 protected:
  void resizeEvent(QResizeEvent*);

 public:
  Window(QWindow* = nullptr);

  inline const Game* game() const { return &m_game; }
  inline Game* game() { return &m_game; }
};

#endif  // WINDOW_HPP
