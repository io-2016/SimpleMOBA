#include "TexturedConvexPolygon.hpp"

TexturedConvexPolygon::TexturedConvexPolygon(SceneGraph::Item* parent)
    : TexturedPolygon(parent) {}

SceneGraph::Node* TexturedConvexPolygon::synchronize(SceneGraph::Node* old) {
  Node* node = static_cast<Node*>(old);

  if (!node) {
    node = new Node(vertices(), textureScale());
  }

  node->setTexture(texture());

  return node;
}

TexturedConvexPolygon::Node::Node(const std::vector<QPointF>& pts,
                                  QVector2D scale)
    : m_geometry({{2, GL_FLOAT}, {2, GL_FLOAT}}, pts.size(),
                 sizeof(QVector4D)) {
  m_geometryNode.setGeometry(&m_geometry);
  m_geometryNode.setMaterial(&m_material);
  appendChild(&m_geometryNode);

  m_geometry.setDrawingMode(GL_TRIANGLE_FAN);

  QVector4D* array = m_geometry.vertexData<QVector4D>();
  for (size_t i = 0; i < pts.size(); i++)
    array[i] = QVector4D(pts[i].x(), pts[i].y(), pts[i].x() * scale.x(),
                         pts[i].y() * scale.y());
  m_geometry.updateVertexData();
}

void TexturedConvexPolygon::Node::setTexture(QOpenGLTexture* t) {
  m_material.setTexture(t);
}
