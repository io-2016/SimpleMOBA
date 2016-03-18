#ifndef CIRCLE_HPP
#define CIRCLE_HPP
#include "Geometry/Vector2d.hpp"
#undef M_PI

const double M_PI = 3.141592653589793;

class Edge;
class b2AABB;

class Circle {
 public:
  float x, y, r;

  Circle();
  Circle(float x, float y, float r);
  Circle(const Vector2d& pos, float r);
  bool isInside(const Vector2d&) const;
  bool colid(const Circle&) const;
  bool colid(const Edge&) const;
  bool colidPoints(const Circle& circle,
                   std::pair<Vector2d, Vector2d>& res) const;
  b2AABB AABB() const;
  Vector2d tangent(const Vector2d& point) const;

  inline Vector2d pos() const { return Vector2d(x, y); }
  inline float radius() const { return r; }
  inline void setCenter(const Vector2d& point) {
    this->x = point.x;
    this->y = point.y;
  }
  inline void setRadius(float r) { this->r = r; }
};

#endif  // CIRCLE_HPP
