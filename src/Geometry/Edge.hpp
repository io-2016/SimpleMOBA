#ifndef EDGE_HPP
#define EDGE_HPP
#include "Vector2d.hpp"
#include <algorithm>
#include <cstdio>
#include <cmath>

class Circle;

class Edge {
 public:
  float x0, y0, x, y;

  Edge();
  Edge(float x1, float y1, float x2, float y2);
  Edge(const Vector2d& begin, const Vector2d& end);
  std::pair<Vector2d, Vector2d> points() const;
  Vector2d vector() const;
  bool colid(const Circle&) const;
  bool colidPoints(const Circle&, std::pair<Vector2d, Vector2d>& res)
      const;  // treats edge like a line
  bool colid(const Edge&) const;
  bool colidPoint(const Edge&, Vector2d&) const;  // treats both edges like
                                                  // lines
  bool insideAABB(const Vector2d&) const;
  bool insideAABB(float, float) const;
  void print() const;
  float length() const;
  float distance(const Vector2d&) const;
  Vector2d vector(float ang, float len) const;
  Vector2d projection(const Vector2d&) const;

  bool operator!=(const Edge&) const;
  bool operator==(const Edge&) const;
  Edge operator-() const;
};

#endif
