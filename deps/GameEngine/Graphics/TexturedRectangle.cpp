#include "TexturedRectangle.hpp"
#include <cassert>

TexturedRectangle::TexturedRectangle(SceneGraph::Item* parent)
    : TexturedItem(parent), m_textureRect(0, 0, 1, 1) {}

void TexturedRectangle::setTextureRect(QRectF rect) {
  if (m_textureRect != rect) {
    m_textureRect = rect;
    update();
  }
}

SceneGraph::Node* TexturedRectangle::synchronize(SceneGraph::Node* old) {
  Node* node = static_cast<Node*>(old);
  if (!node) node = new Node;
  node->synchronize(this);
  return node;
}

TexturedRectangle::Node::Node()
    : m_geometry({{2, GL_FLOAT}, {2, GL_FLOAT}}, 4, sizeof(QVector4D)) {
  m_geometryNode.setGeometry(&m_geometry);
  m_geometryNode.setMaterial(&m_material);
  appendChild(&m_geometryNode);

  m_geometry.setDrawingMode(GL_TRIANGLE_STRIP);
}

void TexturedRectangle::Node::synchronize(const TexturedRectangle* item) {
  updateMaterial(item);
  updateGeometry(item);
}

void TexturedRectangle::Node::updateMaterial(const TexturedRectangle* item) {
  m_material.setTexture(item->texture());
}

void TexturedRectangle::Node::updateGeometry(const TexturedRectangle* item) {
  QRectF trect = item->textureRect();
  QVector4D* array = m_geometry.vertexData<QVector4D>();

  array[0] = QVector4D(0, 0, trect.left(), trect.top());
  array[1] = QVector4D(1, 0, trect.right(), trect.top());
  array[2] = QVector4D(0, 1, trect.left(), trect.bottom());
  array[3] = QVector4D(1, 1, trect.right(), trect.bottom());
  m_geometry.updateVertexData();
}
