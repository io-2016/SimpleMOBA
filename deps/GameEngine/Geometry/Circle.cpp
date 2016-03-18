#include "Circle.hpp"
#include "Vector2d.hpp"
#include "Edge.hpp"
#include <cstdio>
#include <Box2D/Box2D.h>

Circle::Circle() : x(), y(), r() {}

Circle::Circle(float x, float y, float r) : x(x), y(y), r(r) {}

Circle::Circle(const Vector2d& pos, float r) : x(pos.x), y(pos.y), r(r) {}

bool Circle::isInside(const Vector2d& p) const {
  return sqr((double)p.x - x) + sqr((double)p.y - y) < (double)r * r;
}

bool Circle::colid(const Circle& circle) const {
  return (pos() - circle.pos()).lengthSquared() < sqr(circle.r + r);
}

bool Circle::colid(const Edge& edge) const {
  std::pair<Vector2d, Vector2d> res;
  if (edge.colidPoints(*this, res))
    return edge.insideAABB(res.first) || edge.insideAABB(res.second);
  return false;
}

bool Circle::colidPoints(const Circle& circle,
                         std::pair<Vector2d, Vector2d>& res) const {
  float d = (pos() - circle.pos()).length();
  float x1 = (d * d - circle.r * circle.r + r * r) / (2 * d);

  if (r * r - x1 * x1 < 0) return false;

  float y1 = sqrt(r * r - x1 * x1);

  res.first = Vector2d(x1, y1);
  res.first.rotate(atan2(circle.y - y, circle.x - x));
  res.first += Vector2d(x, y);

  res.second = Vector2d(x1, -y1);
  res.second.rotate(atan2(circle.y - y, circle.x - x));
  res.second += Vector2d(x, y);

  return true;
}

b2AABB Circle::AABB() const {
  b2AABB box;
  box.lowerBound = b2Vec2(pos() - Vector2d(r, r));
  box.upperBound = b2Vec2(pos() + Vector2d(r, r));
  return box;
}

Vector2d Circle::tangent(const Vector2d& point) const {
  return (point - pos()).perpendicular();
}
