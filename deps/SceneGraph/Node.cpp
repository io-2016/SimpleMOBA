#include "Node.hpp"
#include "Renderer.hpp"

namespace SceneGraph {

Node::Node(Node* parent, Type type)
    : BaseObject(parent), m_renderer(), m_type(type), m_flag() {}

Node::~Node() {
  if (renderer()) renderer()->nodeDestroyed(this);
}

Node* Node::firstChild() const {
  return static_cast<Node*>(BaseObject::firstChild());
}

Node* Node::next() const { return static_cast<Node*>(BaseObject::next()); }

Node* Node::parent() const { return static_cast<Node*>(BaseObject::parent()); }

void Node::appendChild(Node* node) {
  node->setRenderer(renderer());
  BaseObject::appendChild(node);
}

void Node::removeChild(Node* node) {
  node->setRenderer(nullptr);
  BaseObject::removeChild(node);
}

void Node::setFlag(Flag f) {
  m_flag = f;

  if (renderer() && (f & UsePreprocess)) renderer()->m_preprocess.insert(this);
}

void Node::setRenderer(Renderer* r) {
  if (m_renderer == r) return;

  if (m_renderer) m_renderer->nodeDestroyed(this);

  m_renderer = r;

  if (m_renderer) m_renderer->nodeAdded(this);

  for (Node* node = firstChild(); node; node = node->next())
    node->setRenderer(r);
}

void Node::preprocess() {}

GeometryNode::GeometryNode(Node* parent)
    : Node(parent, Type::GeometryNode), m_material(), m_geometry() {}

TransformNode::TransformNode(Node* parent)
    : Node(parent, Type::TransformNode) {}
}
