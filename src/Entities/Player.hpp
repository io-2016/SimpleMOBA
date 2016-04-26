#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "QBox2D/QBody.hpp"

class Player : public QBody {
 private:
  QPointF m_target;
  bool m_going;

  void onStepped();

 protected:
  void mousePressEvent(QMouseEvent*);

 public:
  Player(SceneGraph::Item* = nullptr);

  void initialize(QWorld* w);

  bool write(QJsonObject&) const;
};

#endif  // PLAYER_HPP
