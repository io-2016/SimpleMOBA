#include "Box2DChain.hpp"
#include "Graphics/Primitives.hpp"
#include <QJsonObject>

Box2DChain::Box2DChain(SceneGraph::Item* parent) : Box2DPolygon(parent) {}

bool Box2DChain::write(QJsonObject& obj) const {
  Box2DPolygon::write(obj);
  obj["class"] = QString("Box2DChain");
  return true;
}

b2Shape* Box2DChain::createShape() const {
  int vertexCount = m_vertices.size() - 1;

  b2ChainShape* chainShape = new b2ChainShape;
  b2Vec2* vertices = new b2Vec2[vertexCount];
  for (int i = 0; i < vertexCount; i++)
    vertices[i].Set(m_vertices[i].x(), m_vertices[i].y());
  chainShape->CreateLoop(vertices, vertexCount);

  delete[] vertices;

  return chainShape;
}

SceneGraph::Node* Box2DChain::synchronize(SceneGraph::Node* old) {
  PolygonNode* node = static_cast<PolygonNode*>(old);
  if (!node) {
    node = new PolygonNode(
        std::vector<QPointF>(m_vertices.begin(), m_vertices.end() - 1));
    node->setColor(Qt::red);
  }

  return node;
}
