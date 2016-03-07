#include "Triangle.hpp"
#include "Triangulate.hpp"
#include <QPointF>

Triangle::Triangle() { m_vertices.resize(3); }

Triangle::Triangle(const QPointF& p1, const QPointF& p2, const QPointF& p3) {
  m_vertices.resize(3);
  m_vertices[0] = p1, m_vertices[1] = p2, m_vertices[2] = p3;
}

void Triangle::setPoint(int id, const QPointF& p) { m_vertices[id] = p; }

const QPointF& Triangle::getPoint(int id) const { return m_vertices[id]; }

const QPointF& Triangle::operator[](int it) const { return m_vertices[it]; }

QPointF& Triangle::operator[](int it) { return m_vertices[it]; }

namespace Geometry {

void triangulate(const std::vector<QPointF>& polygon,
                 std::vector<Triangle>& result) {
  TrStuff::Vector2dVector vec, res;
  for (size_t i = 0; i < polygon.size(); i++)
    vec.push_back(TrStuff::Vector2d(polygon[i].x(), polygon[i].y()));
  TrStuff::Triangulate::Process(vec, res);

  result.clear();
  for (size_t i = 0; i < res.size(); i += 3) {
    Triangle triangle;
    triangle.setPoint(0, QPointF(res[i].GetX(), res[i].GetY()));
    triangle.setPoint(1, QPointF(res[i + 1].GetX(), res[i + 1].GetY()));
    triangle.setPoint(2, QPointF(res[i + 2].GetX(), res[i + 2].GetY()));

    result.push_back(triangle);
  }
}

void toTriangleStrip(std::vector<QPointF> convexPolygon,
                     std::vector<QPointF>& output) {
  output = std::vector<QPointF>(convexPolygon.size());

  size_t i = 1, j = convexPolygon.size() - 1, index = 1;
  bool left = false;

  QPointF p0 = convexPolygon.front();
  output[0] = p0;

  while (index < (size_t)convexPolygon.size()) {
    if (left) {
      output[index] = convexPolygon[i];
      i++;
    } else {
      output[index] = convexPolygon[j];
      j--;
    }
    left = !left, index++;
  }
}

void toTriangles(std::vector<QPointF> polygon, std::vector<QPointF>& output) {
  std::vector<Triangle> triangles;
  Geometry::triangulate(polygon, triangles);

  output.resize(triangles.size() * 3);
  uint id = 0;
  for (uint i = 0; i < triangles.size(); i++)
    for (QPointF p : triangles[i]) output[id++] = p;
}
}
