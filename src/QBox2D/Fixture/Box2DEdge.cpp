#include "Box2DEdge.hpp"
#include "Graphics/Primitives.hpp"

Box2DEdge::Box2DEdge(Item* parent) : QFixture(parent) {}

std::vector<QPointF> Box2DEdge::vertices() const {
  QPointF p1(m_edgeShape.m_vertex1.x, m_edgeShape.m_vertex1.y);
  QPointF p2(m_edgeShape.m_vertex2.x, m_edgeShape.m_vertex2.y);

  return {p1, p2};
}

b2Shape* Box2DEdge::createShape() const { return new b2EdgeShape(m_edgeShape); }

SceneGraph::Node* Box2DEdge::synchronize(SceneGraph::Node* old) {
  EdgeNode* node = static_cast<EdgeNode*>(old);
  if (!node) {
    std::vector<QPointF> v = vertices();
    node = new EdgeNode(v[0], v[1]);
  }

  return node;
}
