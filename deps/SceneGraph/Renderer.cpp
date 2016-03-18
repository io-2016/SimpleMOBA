#include "Renderer.hpp"
#include "Node.hpp"
#include "Geometry.hpp"
#include "Material.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include <cassert>
#include <QColor>
#include <QOpenGLTexture>
#include <QThread>

namespace SceneGraph {

Renderer::Renderer() : m_root(), m_frame(1) {}

Renderer::~Renderer() {
  for (const auto& p : m_texture) delete p.second;
}

void Renderer::updateItem(Item* item) {
  if (item->m_state & Item::ModelMatrixChanged) {
    item->m_itemNode->setMatrix(item->matrix());
    item->m_state &= ~Item::ModelMatrixChanged;

    item->matrixChanged();
  }

  if (item->m_state & Item::ParentChanged) {
    Node* current = item->m_itemNode;
    Node* parent = item->parent() ? item->parent()->m_itemNode : nullptr;
    if (current->parent() != parent && item->visible()) {
      if (current->parent()) current->parent()->removeChild(current);
      if (parent) parent->appendChild(current);
    }

    item->m_state &= ~Item::ParentChanged;
  }

  if (item->m_state & Item::VisibleChanged) {
    item->m_state &= ~Item::VisibleChanged;

    if (item->parent()) {
      assert(item->parent()->m_itemNode);
      if (!item->visible())
        item->parent()->m_itemNode->removeChild(item->m_itemNode);
      else
        item->parent()->m_itemNode->appendChild(item->m_itemNode);
    }
  }

  if (item->visible()) {
    Node* node = item->synchronize(item->m_node);
    if (node != item->m_node) {
      delete item->m_node;
      item->m_node = node;

      if (node && node->parent() == nullptr)
        item->m_itemNode->appendChild(node);
    }
  }
}

void Renderer::updateNodes(Window* window) {
  if (!window->m_updateItem.empty()) window->update();

  for (Item* item : window->m_updateItem) {
    if (item->m_itemNode == nullptr) {
      item->m_itemNode = new TransformNode;
      item->m_itemNode->setRenderer(this);

      item->m_state |= Item::ModelMatrixChanged | Item::ParentChanged;
    }
  }

  std::vector<Item*> nextFrame;
  for (size_t i = 0; i < window->m_updateItem.size(); i++) {
      Item* item = window->m_updateItem[i];
      item->m_state &= ~Item::ScheduledUpdate;
      updateItem(item);

      if (item->m_state & Item::ScheduledUpdate) {
          nextFrame.push_back(item);
          assert(item == window->m_updateItem.back());
          auto it = std::find(window->m_updateItem.rbegin(),
                              window->m_updateItem.rend(), item);
          window->m_updateItem.erase(it.base() - 1);
      }
  }

  window->m_updateItem = nextFrame;
  if (window->m_updateItem.size() > 0) {
    window->scheduleSynchronize();
  }
}

void Renderer::destroyNodes(Window* window) {
  while (!window->m_destroyedItemNode.empty()) {
    Node* itemNode = window->m_destroyedItemNode.back();
    assert(itemNode);
    window->m_destroyedItemNode.pop_back();
    while (itemNode->firstChild())
      itemNode->removeChild(itemNode->firstChild());
    delete itemNode;
  }

  while (!window->m_destroyedNode.empty()) {
    Node* node = window->m_destroyedNode.back();
    window->m_destroyedNode.pop_back();
    delete node;
  }
}

void Renderer::render(Node* root, RenderState state) {
  if (root->type() == Node::Type::GeometryNode) {
    renderGeometryNode(static_cast<GeometryNode*>(root), state);
  } else if (root->type() == Node::Type::TransformNode) {
    TransformNode* node = static_cast<TransformNode*>(root);
    state.setMatrix(state.matrix() * node->matrix());
  }

  for (Node* node = root->firstChild(); node; node = node->next())
    render(node, state);
}

void Renderer::nodeAdded(Node* node) {
  if (node->flag() & Node::UsePreprocess) m_preprocess.insert(node);
}

void Renderer::nodeDestroyed(Node* node) {
  if (node->flag() & Node::UsePreprocess) {
    assert(m_preprocess.find(node) != m_preprocess.end());
    m_preprocess.erase(node);
  }
  if (node == m_root) m_root = nullptr;
}

void Renderer::render() {
  for (Node* node : m_preprocess) node->preprocess();

  render(m_root, m_state);
  m_frame++;
}

void Renderer::synchronize(Window* window) {
  if (m_size != window->size()) {
    setSize(window->size());
    window->update();
  }

  m_state.setMatrix(window->projection());

  updateNodes(window);
  destroyNodes(window);
}

void Renderer::setSize(QSize size) { m_size = size; }

void Renderer::setRoot(Item* item) {
  if (item == nullptr) {
    if (m_root) m_root->setRenderer(nullptr);

    m_root = nullptr;
    return;
  }

  if (!item->m_itemNode) {
    item->m_itemNode = new TransformNode;
    item->m_itemNode->setRenderer(this);
  }

  m_root = item->m_itemNode;
}

QOpenGLTexture* Renderer::texture(const char* path) {
  if (m_texture.find(path) == m_texture.end()) {
    QImage image(path);
    assert(!image.isNull());

    QOpenGLTexture* texture = new QOpenGLTexture(image);
    texture->setMinMagFilters(QOpenGLTexture::Linear, QOpenGLTexture::Linear);
    m_texture[path] = texture;
  }

  return m_texture[path];
}

RenderState::RenderState(QMatrix4x4 m) : m_matrix(m) {}
}
