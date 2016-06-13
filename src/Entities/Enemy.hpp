#ifndef ENEMY_H
#define ENEMY_H

#include "QBox2D/QBody.hpp"

class Enemy : public QBody {
 private:
  QPointF m_target;
  bool m_going;

  double m_radius;
  QPointF center;
  QPointF prev;

  bool m_initialized;

  void onStepped();
  void randomWalk();

 protected:
  void mousePressEvent(QMouseEvent *);

 public:
  Enemy(SceneGraph::Item * = nullptr);

  void initialize(QWorld *w);

  bool write(QJsonObject &) const;

  bool read(const QJsonObject &obj);

  void setRadius(qreal radius);
};

#endif  // ENEMY_H
