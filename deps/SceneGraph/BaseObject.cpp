#include "BaseObject.hpp"
#include <cassert>
#include <QDebug>

namespace SceneGraph {

BaseObject::BaseObject(BaseObject* parent)
    : m_parent(), m_prev(), m_next(), m_firstChild(), m_lastChild() {
  if (parent) parent->appendChild(this);
}

BaseObject::~BaseObject() {
  if (m_parent) m_parent->removeChild(this);

  for (BaseObject* obj = firstChild(); obj; obj = obj->next()) removeChild(obj);
}

void BaseObject::appendChild(BaseObject* node) {
  if (node->m_parent == this) return;

  if (node->m_parent) node->m_parent->removeChild(node);

  node->m_parent = this;
  if (!m_firstChild) m_firstChild = node;
  if (m_lastChild) m_lastChild->m_next = node;
  node->m_prev = m_lastChild;

  m_lastChild = node;
}

void BaseObject::removeChild(BaseObject* node) {
  assert(node->m_parent == this);

  if (m_firstChild == node) m_firstChild = m_firstChild->m_next;
  if (m_lastChild == node) m_lastChild = m_lastChild->m_prev;

  if (node->m_prev) node->m_prev->m_next = node->m_next;
  if (node->m_next) node->m_next->m_prev = node->m_prev;

  node->m_parent = node->m_prev = node->m_next = nullptr;
}
}
