#ifndef ENEMY_H
#define ENEMY_H

#include "QBox2D/QBody.hpp"

class Enemy : public QBody {
 private:
  QPointF m_target;
  bool m_going;
  qreal m_radius;
  QPointF m_center;
  QPointF m_prev;
  bool m_initialized;
  int m_hitpoints;

  void onStepped();
  void randomWalk();

 protected:
  void mousePressEvent(QMouseEvent *);
  void beginContact(QFixture *other, b2Contact *);

 public:
  Enemy(SceneGraph::Item * = nullptr);

  void initialize(QWorld *w);

  bool write(QJsonObject &) const;

  bool read(const QJsonObject &obj);

  void setRadius(qreal radius);
};

#endif  // ENEMY_H
