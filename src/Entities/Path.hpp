#ifndef PATH_H
#define PATH_H

#include <QPointF>
#include <vector>

class World;

class Path {
 public:
  Path(QPointF a, QPointF b, World *w);

  const std::vector<QPointF> &points() const;

 private:
  std::vector<QPointF> m_points;
};

#endif  // PATH_H
