#include "Primitives.hpp"
#include "Geometry/Triangle.hpp"
#include "Geometry/Circle.hpp"

ConvexPolygonGeometry::ConvexPolygonGeometry(const std::vector<QPointF>& pts)
    : SceneGraph::Geometry({{2, GL_FLOAT}}, pts.size(), sizeof(QVector2D)) {
  setDrawingMode(GL_TRIANGLE_FAN);
  for (size_t i = 0; i < pts.size(); i++)
    vertexData<QVector2D>()[i] = QVector2D(pts[i].x(), pts[i].y());
  updateVertexData();
}

ConvexPolygonNode::ConvexPolygonNode(const std::vector<QPointF>& pts)
    : m_geometry(pts) {
  setGeometry(&m_geometry);
  setMaterial(&m_material);
}

void ConvexPolygonNode::setColor(QColor color) { m_material.setColor(color); }

PolygonGeometry::PolygonGeometry(const std::vector<QPointF>& pts)
    : SceneGraph::Geometry({{2, GL_FLOAT}}, 3 * (pts.size() - 2),
                           sizeof(QVector2D)) {
  std::vector<Triangle> triangles;
  ::Geometry::triangulate(pts, triangles);

  setDrawingMode(GL_TRIANGLES);

  int index = 0;
  for (const Triangle& tri : triangles)
    for (const QPointF& p : tri)
      vertexData<QVector2D>()[index++] = QVector2D(p.x(), p.y());
  updateVertexData();
}

PolygonNode::PolygonNode(const std::vector<QPointF>& pts) : m_geometry(pts) {
  setGeometry(&m_geometry);
  setMaterial(&m_material);
}

void PolygonNode::setColor(QColor color) { m_material.setColor(color); }

EdgeNode::EdgeNode() : EdgeNode(QPointF(), QPointF()) {}

EdgeNode::EdgeNode(QPointF p1, QPointF p2)
    : m_geometry({{2, GL_FLOAT}}, 2, sizeof(QVector2D)) {
  setGeometry(&m_geometry);
  setMaterial(&m_material);

  m_geometry.setDrawingMode(GL_LINES);

  setP1(p1);
  setP2(p2);
}

void EdgeNode::setP1(QPointF p1) {
  m_geometry.vertexData<QVector2D>()[0] = QVector2D(p1.x(), p1.y());
  m_geometry.updateVertexData();
}

void EdgeNode::setP2(QPointF p2) {
  m_geometry.vertexData<QVector2D>()[0] = QVector2D(p2.x(), p2.y());
  m_geometry.updateVertexData();
}

void EdgeNode::setColor(QColor color) { m_material.setColor(color); }

CircleNode::CircleNode(QPointF pos, float radius, int accuracy) {
  std::vector<QPointF> points(accuracy);

  for (int i = 0; i < accuracy; i++) {
    qreal angle = i * 2 * M_PI / accuracy;
    qreal x = cos(angle) * radius + pos.x();
    qreal y = sin(angle) * radius + pos.y();

    points[i] = {x, y};
  }

  setGeometry(m_geometry = new ConvexPolygonGeometry(points));
  setMaterial(&m_material);
}

CircleNode::CircleNode(const Circle& circle, int accuracy)
    : CircleNode((QPointF)circle.pos(), circle.radius(), accuracy) {}

void CircleNode::setColor(QColor color) { m_material.setColor(color); }

CircleNode::~CircleNode() { delete m_geometry; }
