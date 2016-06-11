#ifndef PATH_H
#define PATH_H

#include <QPointF>
#include <vector>
#include "GameEngine/QBox2D/QBody.hpp"

class QWorld;

class Path {
 public:
  Path(QPointF a, QPointF b, QBody *skip, qreal radius, QWorld *w);

  const std::vector<QPointF> &points() const;

  void build(QPointF a, QPointF b, QBody *skip, qreal radius, QWorld *w);

 private:
  std::vector<QPointF> m_points;
};

#endif  // PATH_H
