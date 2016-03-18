#include "TexturedPolygon.hpp"
#include "Graphics/Primitives.hpp"
#include "Geometry/Triangle.hpp"

TexturedPolygon::TexturedPolygon(SceneGraph::Item* parent)
    : TexturedItem(parent), m_textureScale(1, 1) {}

SceneGraph::Node* TexturedPolygon::synchronize(SceneGraph::Node* old) {
  Node* node = static_cast<Node*>(old);

  if (!node) node = new Node(vertices(), textureScale());

  node->setTexture(texture());

  return node;
}

TexturedPolygon::Node::Node(const std::vector<QPointF>& pts, QVector2D scale)
    : m_geometry({{2, GL_FLOAT}, {2, GL_FLOAT}}, 3 * (pts.size() - 2),
                 sizeof(QVector4D)) {
  m_geometryNode.setGeometry(&m_geometry);
  m_geometryNode.setMaterial(&m_material);
  appendChild(&m_geometryNode);

  m_geometry.setDrawingMode(GL_TRIANGLES);

  QVector4D* array = m_geometry.vertexData<QVector4D>();
  std::vector<QPointF> p;
  Geometry::toTriangles(pts, p);
  for (size_t i = 0; i < p.size(); i++)
    array[i] = QVector4D(p[i].x(), p[i].y(), p[i].x() * scale.x(),
                         p[i].y() * scale.y());
  m_geometry.updateVertexData();
}

void TexturedPolygon::Node::setTexture(QOpenGLTexture* texture) {
  m_material.setTexture(texture);
}
