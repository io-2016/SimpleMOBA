#include "AddPolygon.hpp"
#include "QBox2D/QWorld.hpp"
#include "Geometry/Functions.hpp"
#include "Graphics/Primitives.hpp"
#include "QBox2D/Fixture/Box2DPolygon.hpp"
#include "AddBody.hpp"

AddPolygon::AddPolygon(AddBody* parent) : AddFixture(parent), m_object(this) {}

void AddPolygon::reset() {
  m_pts.clear();
  update();
}

QFixture* AddPolygon::fixture() const {
  if (m_pts.size() < 3) return nullptr;
  Box2DPolygon* polygon = new Box2DPolygon;
  polygon->setVertices(m_pts);

  return polygon;
}

void AddPolygon::mousePressEvent(QMouseEvent*) {}

void AddPolygon::mouseReleaseEvent(QMouseEvent* event) {
  m_pts.push_back(mapFromScreen(event->pos()));

  if (m_pts.size() >= 3) {
    std::vector<Vector2d> vec;
    for (QPointF p : m_pts) vec.push_back(Vector2d(p));

    Geometry::convexHull(vec);

    m_pts.clear();
    for (Vector2d p : vec) m_pts.push_back(QPointF(p));
  }

  update();
}

SceneGraph::Node* AddPolygon::synchronize(SceneGraph::Node*) {
  if (m_pts.size() < 3) {
    return new Node(m_pts, QSizeF(1, 1));
  }

  ConvexPolygonNode* node = new ConvexPolygonNode(m_pts);
  node->setColor(Qt::yellow);

  return node;
}

AddPolygon::Node::Node(std::vector<QPointF> pts, QSizeF size) {
  qreal w = size.width(), h = size.height();
  for (QPointF p : pts) {
    std::vector<QPointF> vertex = {QPointF(-0.5 * w + p.x(), -0.5 * h + p.y()),
                                   QPointF(0.5 * w + p.x(), -0.5 * h + p.y()),
                                   QPointF(0.5 * w + p.x(), 0.5 * h + p.y()),
                                   QPointF(-0.5 * w + p.x(), 0.5 * h + p.y())};

    ConvexPolygonNode* node = new ConvexPolygonNode(vertex);
    appendChild(node);

    m_node.push_back(node);
  }
}

AddPolygon::Node::~Node() {
  for (SceneGraph::Node* node : m_node) delete node;
}
