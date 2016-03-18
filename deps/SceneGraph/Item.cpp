#include "Item.hpp"
#include "Window.hpp"
#include "Node.hpp"
#include "Renderer.hpp"
#include "Window.hpp"
#include <cassert>

namespace SceneGraph {

Item::Item(Item* parent)
    : BaseObject(parent),
      m_window(),
      m_itemNode(),
      m_node(),
      m_state(ParentChanged),
      m_lastUpdate(0) {
  if (parent && parent->window()) {
    setWindow(parent->window());
  }

  setVisible(true);
}

Item::~Item() { setWindow(nullptr); }

Item* Item::firstChild() const {
  return static_cast<Item*>(BaseObject::firstChild());
}

Item* Item::next() const { return static_cast<Item*>(BaseObject::next()); }

Item* Item::parent() const { return static_cast<Item*>(BaseObject::parent()); }

void Item::setParent(Item* item) {
  if (parent() != item) {
    if (parent()) parent()->removeChild(this);
    if (item) item->appendChild(this);
  }
}

void Item::appendChild(Item* item) {
  assert(item);
  item->setWindow(window());

  item->m_state |= ParentChanged;
  item->update();

  BaseObject::appendChild(item);
}

void Item::removeChild(Item* item) {
  item->m_state |= ParentChanged;
  item->update();

  item->setWindow(nullptr);
  BaseObject::removeChild(item);
}

void Item::setMatrix(const QMatrix4x4& m) {
  m_matrix = m;
  m_state |= ModelMatrixChanged;
  update();
}

void Item::resetTransform() {
  m_matrix.setToIdentity();
  m_state |= ModelMatrixChanged;
  update();
}

void Item::translate(qreal x, qreal y) {
  m_matrix.translate(x, y);
  m_state |= ModelMatrixChanged;
  update();
}

void Item::scale(qreal x, qreal y) {
  m_matrix.scale(x, y);
  m_state |= ModelMatrixChanged;
  update();
}

void Item::rotate(qreal angle, qreal x, qreal y, qreal z) {
  m_matrix.rotate(angle, x, y, z);
  m_state |= ModelMatrixChanged;
  update();
}

QMatrix4x4 Item::effectiveMatrix() const {
  QMatrix4x4 matrix;
  const Item* item = this;
  while (item) {
    matrix = item->matrix() * matrix;
    item = item->parent();
  }

  return matrix;
}

QPointF Item::mapToItem(Item* item, QPointF p) {
  return item->effectiveMatrix() * mapToScreen(p);
}

QPointF Item::mapToScreen(QPointF p) { return effectiveMatrix() * p; }

QPointF Item::mapFromItem(Item* item, QPointF p) {
  return item->mapToItem(this, p);
}

QPointF Item::mapFromScreen(QPointF p) {
  return effectiveMatrix().inverted() * p;
}

void Item::setFocus(bool enabled) {
  unsigned state = m_state;
  if (enabled)
    m_state |= HasFocus;
  else
    m_state &= ~HasFocus;

  if (m_state != state) {
    assert(window());

    if (enabled) {
      if (window()->m_focusItem) window()->m_focusItem->setFocus(false);
      window()->m_focusItem = this;
    } else {
      if (window()->m_focusItem == this) window()->m_focusItem = nullptr;
    }

    focusChanged();
  }
}

void Item::setVisible(bool enabled) {
  unsigned state = m_state;
  if (enabled)
    state |= Visible;
  else
    state &= ~Visible;

  if (m_state != state) {
    m_state = state ^ VisibleChanged;
    update();

    visibleChanged();
  }
}

int Item::startTimer(int interval) {
  assert(window());
  return window()->installTimer(this, interval);
}

void Item::killTimer(int timerId) {
  assert(window());
  window()->removeTimer(this, timerId);
}

void Item::setWindow(Window* window) {
  if (m_window == window) return;
  if (m_window) m_window->onItemDestroyed(this);

  m_window = window;
  update();

  for (Item* item = firstChild(); item; item = item->next())
    item->setWindow(window);
}

Node* Item::synchronize(Node*) { return nullptr; }

void Item::visibleChanged() {}

void Item::focusChanged() {}

void Item::matrixChanged() {}

void Item::keyPressEvent(QKeyEvent* e) { e->ignore(); }

void Item::keyReleaseEvent(QKeyEvent* e) { e->ignore(); }

void Item::touchEvent(QTouchEvent* e) { e->ignore(); }

void Item::mousePressEvent(QMouseEvent* e) { e->ignore(); }

void Item::mouseReleaseEvent(QMouseEvent* e) { e->ignore(); }

void Item::mouseMoveEvent(QMouseEvent* e) { e->ignore(); }

void Item::wheelEvent(QWheelEvent* e) { e->ignore(); }

void Item::timerEvent(QTimerEvent*) {}

void Item::update() {
  if (window()) {
    window()->scheduleUpdate(this);
    window()->scheduleSynchronize();
  }
}

void Item::invalidate() {
  if (window()) {
    window()->destroyNode(this);
    window()->cancelUpdate(this);
  }
}

void Item::updateSubtree() {
  update();
  for (Item* i = firstChild(); i; i = i->next())
    i->updateSubtree();
}

void Item::invalidateSubtree() {
  invalidate();
  for (Item* i = firstChild(); i; i = i->next()) i->invalidateSubtree();
}

}
