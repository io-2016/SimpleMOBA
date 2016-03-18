#include "Edge.hpp"
#include "Functions.hpp"
#include "Circle.hpp"
typedef long double LD;

using std::max;
using std::min;

Edge::Edge() { x = y = x0 = y0 = 0; }

Edge::Edge(float x1, float y1, float x2, float y2) {
  x = x2 - x1, y = y2 - y1;
  x0 = x1, y0 = y1;
}

Edge::Edge(const Vector2d& begin, const Vector2d& end) {
  *this = Edge(begin.x, begin.y, end.x, end.y);
}

std::pair<Vector2d, Vector2d> Edge::points() const {
  return std::make_pair(Vector2d(x0, y0), Vector2d(x0 + x, y0 + y));
}

Vector2d Edge::vector() const { return Vector2d(x, y); }

bool Edge::colid(const Circle& circle) const {
  std::pair<Vector2d, Vector2d> res;
  if (colidPoints(circle, res))
    if (insideAABB(res.first) || insideAABB(res.second)) return true;

  return false;
}

bool Edge::colidPoints(const Circle& circle,
                       std::pair<Vector2d, Vector2d>& res) const {
  double _x = x0 - circle.x, _y = y0 - circle.y, r = circle.r;

  double a = ((double)x * x + (double)y * y), b = 2.f * (_x * x + _y * y),
         c = _x * _x + _y * _y - r * r;
  double delta = b * b - 4 * a * c;
  if (delta < 0) return false;

  double k1 = (-b - sqrt(delta)) / (2 * a);
  double k2 = (-b + sqrt(delta)) / (2 * a);

  res.first.x = k1 * x + x0;
  res.first.y = k1 * y + y0;
  res.second.x = k2 * x + x0;
  res.second.y = k2 * y + y0;

  return true;
}

bool Edge::colid(const Edge& a) const {
  Vector2d p1 = Vector2d(x0, y0), p2 = Vector2d(x0 + x, y0 + y);
  Vector2d p3 = Vector2d(a.x0, a.y0), p4 = Vector2d(a.x0 + a.x, a.y0 + a.y);

  float d1 = Geometry::det3(p3, p4, p1), d2 = Geometry::det3(p3, p4, p2),
        d3 = Geometry::det3(p1, p2, p3), d4 = Geometry::det3(p1, p2, p4);

  if (d1 * d2 < 0 && d3 * d4 < 0) return true;
  if (fabs(d1) < EPS && a.insideAABB(p1)) return true;
  if (fabs(d2) < EPS && a.insideAABB(p2)) return true;
  if (fabs(d3) < EPS && insideAABB(p3)) return true;
  if (fabs(d4) < EPS && insideAABB(p4)) return true;
  return false;
}

bool Edge::colidPoint(const Edge& a, Vector2d& res) const {
  float W = (-x) * a.y - a.x * (-y);
  float WK = (a.x0 + a.x - x0 - x) * (a.y) - (a.x) * (a.y0 + a.y - y0 - y);

  if (fabs(W) < EPS) return false;

  float k = WK / W;
  float x_c = k * x0 + (1 - k) * (x0 + x), y_c = k * y0 + (1 - k) * (y0 + y);

  res = Vector2d(x_c, y_c);

  return true;
}

bool Edge::insideAABB(float my_x, float my_y) const {
  return my_x <= max(x0, x0 + x) && my_x >= min(x0, x0 + x) &&
         my_y <= max(y0, y0 + y) && my_y >= min(y0, y0 + y);
}

bool Edge::insideAABB(const Vector2d& a) const { return insideAABB(a.x, a.y); }

void Edge::print() const {
  printf("%.8f %.8f %.8f %.8f\n", x0, y0, x0 + x, y0 + y);
}

float Edge::length() const { return sqrt(x * x + y * y); }

bool Edge::operator!=(const Edge& a) const { return !(*this == a); }

bool Edge::operator==(const Edge& a) const {
  return ((Vector2d(x0, y0) == Vector2d(a.y0, a.y0)) &&
          (Vector2d(x, y) == Vector2d(a.x, a.y)));
}

Edge Edge::operator-() const { return Edge(x0 + x, y0 + y, x0, y0); }

Vector2d Edge::vector(float ang, float len) const {
  Vector2d res =
      points().second + Geometry::normalizedToLength(Vector2d(x, y), len);
  res.rotate(M_PI - ang, points().second);

  return res - points().second;
}

Vector2d Edge::projection(const Vector2d& a) const {
  float k = (x * (a.x - x0) + y * (a.y - y0)) / (x * x + y * y);

  return Vector2d(x0, y0) + k * Vector2d(x, y);
}

float Edge::distance(const Vector2d& p) const {
  return (projection(p) - p).length();
}
