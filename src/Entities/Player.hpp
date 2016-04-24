#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "QBox2D/QBody.hpp"
#include "Utility/Path.hpp"

class Player : public QBody {
 private:
  QPointF m_target;
  std::unique_ptr<Path> m_currentPath;
  int m_currentPathPoint;
  bool m_going;

  void onStepped();

 protected:
  void mousePressEvent(QMouseEvent*);

 public:
  Player(SceneGraph::Item* = nullptr);

  void initialize(QWorld* w);
};

#endif  // PLAYER_HPP
