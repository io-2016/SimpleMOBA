#ifndef PATH_H
#define PATH_H

#include <QPointF>
#include <vector>

class World;

class Path {
 public:
  Path(QPointF a, QPointF b, World *w);

  const std::vector<QPointF> &points() const;

  // Retrieve the point on the path from the fraction of the total length.
  QPointF at(float factor) const;

  float length() const;

 private:
  std::vector<QPointF> m_points;
};

#endif  // PATH_H
