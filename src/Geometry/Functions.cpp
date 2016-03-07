#include "Functions.hpp"
#include "Edge.hpp"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cassert>

Vector2d Geometry::normalizedToLength(Vector2d vec, float length) {
  float angle = atan2(vec.y, vec.x);

  return Vector2d(length * cos(angle), length * sin(angle));
}

bool Geometry::isLower(const Vector2d &low, const Vector2d &high) {
  if (fabs(low.y - high.y) < EPS && low.x < high.x) return true;
  return low.y < high.y;
}

bool Geometry::angleCmp(const Vector2d &a, const Vector2d &b) {
  if (fabs((double)a.x * b.y - (double)a.y * b.x) < EPS)
    return a.lengthSquared() < b.lengthSquared();

  return (double)a.x * b.y > (double)a.y * b.x;
}

void Geometry::uniquePoints(std::vector<Vector2d> &verts) {
  std::sort(verts.begin(), verts.end(), Geometry::angleCmp);
  verts.resize(std::unique(verts.begin(), verts.end()) - verts.begin());
}

Geometry::CmpAngleSort::CmpAngleSort(float minX, float minY)
    : m_lowest(minX, minY) {}

Geometry::CmpAngleSort::CmpAngleSort(const Vector2d &mid) : m_lowest(mid) {}

bool Geometry::CmpAngleSort::operator()(const Vector2d &a,
                                        const Vector2d &b) const {
  return Geometry::angleCmp(a - m_lowest, b - m_lowest);
}

void Geometry::convexHull(std::vector<Vector2d> &verts) {
  assert(verts.size() >= 3);
  Geometry::angleSort(verts.begin(), verts.end());

  std::vector<Vector2d> res;
  for (size_t i = 0; i < verts.size(); i++) {
    while (res.size() >= 2 &&
           det3(res[res.size() - 2], res[res.size() - 1], verts[i]) < EPS)
      res.pop_back();
    res.push_back(verts[i]);
  }

  verts = res;
}

bool Geometry::inRange(float a, float b1, float b2) {
  if (fabs(a - b1) < 1e-9 || fabs(a - b2) < 1e-9) return true;
  return a >= std::min(b1, b2) && a <= std::max(b1, b2);
}

bool Geometry::collision1d(float a1, float a2, float b1, float b2) {
  return Geometry::inRange(a1, b1, b2) || Geometry::inRange(a2, b1, b2) ||
         Geometry::inRange(b1, a1, a2) || Geometry::inRange(b2, a1, a2);
}

std::vector<Vector2d> Geometry::simplifyPolygon(std::vector<Vector2d> polygon,
                                                float eps) {
  if (polygon.size() < 3) return polygon;

  Edge line(polygon.front(), polygon.back());
  auto cmpDist = [line](const Vector2d &p1, const Vector2d &p2) {
    return line.distance(p1) < line.distance(p2);
  };

  size_t index =
      std::max_element(polygon.begin() + 1, polygon.end() - 1, cmpDist) -
      polygon.begin();

  if (line.distance(polygon[index]) > eps) {
    std::vector<Vector2d> part1, part2, res;
    for (size_t i = 0; i <= index; i++) part1.push_back(polygon[i]);
    for (size_t i = index + 1; i < polygon.size(); i++)
      part2.push_back(polygon[i]);

    part1 = Geometry::simplifyPolygon(part1, eps);
    part2 = Geometry::simplifyPolygon(part2, eps);

    for (size_t i = 0; i < part1.size(); i++) res.push_back(part1[i]);
    for (size_t i = 0; i < part2.size(); i++) res.push_back(part2[i]);
    return res;
  }

  return std::vector<Vector2d>{polygon.front(), polygon.back()};
}
