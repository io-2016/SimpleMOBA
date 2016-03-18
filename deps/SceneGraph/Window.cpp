#include "Window.hpp"
#include "DefaultRenderer.hpp"
#include "Node.hpp"
#include <functional>
#include <QThread>
#include <QQuickItem>
#include <QOpenGLFramebufferObject>
#include <cassert>

namespace SceneGraph {

Window::Window(QWindow* parent)
    : QQuickView(parent),
      m_rootItem(this, contentItem()),
      m_renderer(),
      m_focusItem() {
  m_root.setWindow(this);

  connect(this, &QQuickWindow::sceneGraphInitialized, this,
          &Window::onSceneGraphInitialized, Qt::DirectConnection);
  connect(this, &QQuickWindow::sceneGraphInvalidated, this,
          &Window::onSceneGraphInvalidated, Qt::DirectConnection);
  connect(this, &QQuickWindow::beforeRendering, this,
          &Window::onBeforeRendering, Qt::DirectConnection);
  connect(this, &QQuickWindow::beforeSynchronizing, this,
          &Window::onBeforeSynchronizing, Qt::DirectConnection);

  setResizeMode(SizeRootObjectToView);
  setClearBeforeRendering(false);
  setPersistentSceneGraph(false);
}

Window::~Window() {
  onSceneGraphInvalidated();
  m_root.setWindow(nullptr);
  disconnect(this, &QQuickWindow::sceneGraphInitialized, this,
             &Window::onSceneGraphInitialized);
  disconnect(this, &QQuickWindow::sceneGraphInvalidated, this,
             &Window::onSceneGraphInvalidated);
  disconnect(this, &QQuickWindow::beforeRendering, this,
             &Window::onBeforeRendering);
  disconnect(this, &QQuickWindow::beforeSynchronizing, this,
             &Window::onBeforeSynchronizing);
}

void Window::setProjection(const QMatrix4x4& m) {
  m_projection = m;
  scheduleSynchronize();
}

void Window::scheduleSynchronize() { update(); }

QOpenGLTexture* Window::texture(const char* path) {
  return m_renderer->texture(path);
}

void Window::onSceneGraphInitialized() {
  m_renderer = new DefaultRenderer;
  m_renderer->setRoot(rootItem());
  rootItem()->updateSubtree();
}

void Window::onSceneGraphInvalidated() {
  if (!m_renderer) return;
  rootItem()->invalidateSubtree();
  m_renderer->synchronize(this);

  delete m_renderer;
  m_renderer = nullptr;
}

void Window::onBeforeRendering() {
  resetOpenGLState();
  m_renderer->render();
}

void Window::onBeforeSynchronizing() { m_renderer->synchronize(this); }

void Window::onItemDestroyed(Item* item) {
  cancelUpdate(item);

  if (m_timerMap.find(item) != m_timerMap.end()) {
    std::unordered_set<int> timer = m_timerMap[item];
    for (int t : timer) removeTimer(item, t);
  }

  destroyNode(item);

  if (m_focusItem == item) m_focusItem = nullptr;

  scheduleSynchronize();
}

void Window::destroyNode(Item* item) {
  if (item->m_itemNode) m_destroyedItemNode.push_back(item->m_itemNode);
  if (item->m_node) m_destroyedNode.push_back(item->m_node);

  item->m_itemNode = nullptr;
  item->m_node = nullptr;
}

void Window::scheduleUpdate(Item* item) {
  if (!(item->m_state & Item::ScheduledUpdate)) {
    m_updateItem.push_back(item);
    item->m_state |= Item::ScheduledUpdate;
  } else {
    assert(std::find(m_updateItem.begin(), m_updateItem.end(), item) !=
           m_updateItem.end());
  }
}

void Window::cancelUpdate(Item* item) {
  if (item->m_state & Item::ScheduledUpdate) {
    auto it = std::find(m_updateItem.begin(), m_updateItem.end(), item);
    if (it != m_updateItem.end()) m_updateItem.erase(it);

    item->m_state &= ~Item::ScheduledUpdate;
  }
}

int Window::installTimer(Item* item, int interval) {
  int id = startTimer(interval);
  m_timerMap[item].insert(id);
  m_timerItem[id] = item;

  return id;
}

void Window::removeTimer(Item* item, int timerId) {
  std::unordered_set<int>& set = m_timerMap[item];
  assert(set.find(timerId) != set.end());

  set.erase(set.find(timerId));
  killTimer(timerId);

  if (set.empty()) m_timerMap.erase(m_timerMap.find(item));
}

void Window::keyPressEvent(QKeyEvent* event) {
  QQuickView::keyPressEvent(event);
  if (event->isAccepted()) return;

  Item* item = focusItem();
  while (item) {
    event->accept();
    item->keyPressEvent(event);
    if (event->isAccepted()) break;
    item = item->parent();
  }

  if (!item) event->ignore();
}

void Window::keyReleaseEvent(QKeyEvent* event) {
  QQuickView::keyReleaseEvent(event);
  if (event->isAccepted()) return;

  Item* item = focusItem();
  while (item) {
    event->accept();
    item->keyReleaseEvent(event);
    if (event->isAccepted()) break;
    item = item->parent();
  }

  if (!item) event->ignore();
}

void Window::touchEvent(QTouchEvent* e) {
  QQuickView::touchEvent(e);
  if (e->isAccepted()) return;

  Item* item = focusItem();
  while (item) {
    e->accept();
    item->touchEvent(e);
    if (e->isAccepted())
      break;
    else {
      bool accepted = false;
      if (e->touchPoints().size() == 1) {
        QTouchEvent::TouchPoint p = e->touchPoints().front();
        if (e->touchPointStates() & Qt::TouchPointPressed) {
          QMouseEvent t(QEvent::MouseButtonPress, p.pos(), Qt::LeftButton,
                        Qt::LeftButton, Qt::NoModifier);
          item->mousePressEvent(&t);
          accepted |= t.isAccepted();
        }
        if (e->touchPointStates() & Qt::TouchPointMoved) {
          QMouseEvent t(QEvent::MouseMove, p.pos(), Qt::NoButton, Qt::NoButton,
                        Qt::NoModifier);
          item->mouseMoveEvent(&t);
          accepted |= t.isAccepted();
        }
        if (e->touchPointStates() & Qt::TouchPointReleased) {
          QMouseEvent t(QEvent::MouseButtonRelease, p.pos(), Qt::LeftButton,
                        Qt::LeftButton, Qt::NoModifier);
          item->mouseReleaseEvent(&t);
          accepted |= t.isAccepted();
        }
      }

      if (accepted) break;
    }
    item = item->parent();
  }

  if (!item) e->ignore();
}

void Window::mousePressEvent(QMouseEvent* event) {
  QQuickView::mousePressEvent(event);
  if (event->isAccepted()) return;

  Item* item = focusItem();
  while (item) {
    event->accept();
    item->mousePressEvent(event);
    if (event->isAccepted()) break;
    item = item->parent();
  }

  if (!item) event->ignore();

  if (rootObject())
    rootObject()->forceActiveFocus();
}

void Window::mouseReleaseEvent(QMouseEvent* event) {
  QQuickView::mouseReleaseEvent(event);
  if (event->isAccepted()) return;

  Item* item = focusItem();
  while (item) {
    event->accept();
    item->mouseReleaseEvent(event);
    if (event->isAccepted()) break;
    item = item->parent();
  }

  if (!item) event->ignore();
}

void Window::mouseMoveEvent(QMouseEvent* event) {
  QQuickView::mouseMoveEvent(event);
  if (event->isAccepted()) return;

  Item* item = focusItem();
  while (item) {
    event->accept();
    item->mouseMoveEvent(event);
    if (event->isAccepted()) break;
    item = item->parent();
  }

  if (!item) event->ignore();
}

void Window::wheelEvent(QWheelEvent* event) {
  QQuickView::wheelEvent(event);
  if (event->isAccepted()) return;

  Item* item = focusItem();
  while (item) {
    event->accept();
    item->wheelEvent(event);
    if (event->isAccepted()) break;
    item = item->parent();
  }

  if (!item) event->ignore();
}

void Window::timerEvent(QTimerEvent* event) {
  QQuickView::timerEvent(event);

  auto it = m_timerItem.find(event->timerId());
  if (it != m_timerItem.end()) it->second->timerEvent(event);
}

void Window::resizeEvent(QResizeEvent* event) {
  QQuickView::resizeEvent(event);
  m_rootItem.setSize(event->size());
  scheduleSynchronize();
}

Window::RootItem::RootItem(Window* w, QQuickItem* parent)
    : QQuickItem(parent), m_window(w) {}

void Window::RootItem::touchEvent(QTouchEvent* e) {
  m_window->touchEvent(e);
  e->accept();
}
}
