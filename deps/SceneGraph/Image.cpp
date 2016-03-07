#include "Image.hpp"
#include "Window.hpp"

namespace SceneGraph {

Image::Image(Item* parent) : Item(parent) {}

Node* Image::synchronize(Node* node) {
  ImageNode* root = static_cast<ImageNode*>(node);

  if (root == nullptr) {
    root = new ImageNode;
    root->setTexture(window()->texture(source().c_str()));
  }

  return root;
}

Image::ImageNode::ImageNode(Node* parent)
    : GeometryNode(parent),
      m_geometry({{2, GL_FLOAT}, {2, GL_FLOAT}}, 4, sizeof(Vertex)) {
  setGeometry(&m_geometry);
  setMaterial(&m_material);

  m_geometry.vertexData<Vertex>()[0] = {0, 0, 0, 0};
  m_geometry.vertexData<Vertex>()[1] = {0, 1, 0, 1};
  m_geometry.vertexData<Vertex>()[2] = {1, 0, 1, 0};
  m_geometry.vertexData<Vertex>()[3] = {1, 1, 1, 1};
  m_geometry.updateVertexData();
}

void Image::ImageNode::setTexture(QOpenGLTexture* t) {
  m_material.setTexture(t);
}
}
