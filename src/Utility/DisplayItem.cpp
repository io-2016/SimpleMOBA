#include "DisplayItem.hpp"
#include "BaseItem.hpp"
#include "SceneGraph/Window.hpp"

DisplayItem::DisplayItem(DisplayItemFrame* parent)
    : SceneGraph::Item(parent),
      m_frame(parent),
      m_focusedObject(),
      m_flickable(),
      m_buttonDown(),
      m_distance(),
      m_factor(1) {
  m_frame->m_displayItem = this;
}

DisplayItem::~DisplayItem() {
  if (m_focusedObject) m_focusedObject->m_displayItem = nullptr;
  if (m_frame) m_frame->m_displayItem = nullptr;
}

void DisplayItem::setSize(QSizeF size) {
  if (size != m_size) {
    m_size = size;
    updateVisibleArea();
  }
}

void DisplayItem::setFactor(qreal f) {
  if (!qFuzzyIsNull(m_factor - f)) {
    m_factor = f;
    updateVisibleArea();
  }
}

void DisplayItem::setVisibleArea(QRectF rect) {
  if (m_visibleArea == rect) return;
  m_visibleArea = rect;
  visibleAreaChanged();
}

void DisplayItem::setEffectiveLookAt(QPointF p) {
  if (m_effectiveLookAt == p) return;
  m_effectiveLookAt = p;

  updateVisibleArea();
}

void DisplayItem::setLookAt(QPointF p) {
  if (m_lookAt == p) return;
  m_lookAt = p;

  returnLookAtToBounds();
}

void DisplayItem::setFocusedObject(BaseItem* item) {
  if (m_focusedObject == item) return;

  if (focusedObject()) {
    focusedObject()->m_displayItem = nullptr;
  }

  m_focusedObject = item;

  if (focusedObject()) {
    focusedObject()->m_displayItem = this;
    focusedObjectPositionChanged();
  }
}

void DisplayItem::setFlickable(bool enable) { m_flickable = enable; }

void DisplayItem::updateVisibleArea() {
  returnScaleToBounds();
  returnLookAtToBounds();

  resetTransform();
  translate(0.5 * m_frame->size().width(), 0.5 * m_frame->size().height());
  scale(factor(), factor());
  translate(-effectiveLookAt().x(), -effectiveLookAt().y());

  QRectF rect(QPointF(), m_frame->size());
  setVisibleArea(matrix().inverted().mapRect(rect));
}

void DisplayItem::focusedObjectPositionChanged() {
  setLookAt(-focusedObject()->matrix() * QPointF(0, 0));
}

void DisplayItem::returnLookAtToBounds() {
  QPointF p = lookAt();
  qreal fwidth = m_frame->size().width();
  qreal fheight = m_frame->size().height();
  qreal width = size().width();
  qreal height = size().height();

  p.rx() = std::max(p.x(), fwidth / (2 * factor()));
  p.rx() = std::min(p.x(), width - fwidth / (2 * factor()));
  p.ry() = std::max(p.y(), fheight / (2 * factor()));
  p.ry() = std::min(p.y(), height - fheight / (2 * factor()));

  setEffectiveLookAt(p);
}

void DisplayItem::returnScaleToBounds() {
  qreal w = size().width();
  qreal h = size().height();
  qreal aspect =
      std::max(m_frame->size().width() / w, m_frame->size().height() / h);

  setFactor(std::max(factor(), (qreal)aspect));
}

void DisplayItem::wheelEvent(QWheelEvent* event) {
  if (event->angleDelta().y() > 0)
    setFactor(factor() * 1.1);
  else
    setFactor(factor() / 1.1);
}

void DisplayItem::mousePressEvent(QMouseEvent* event) {
  if (event->button() != Qt::LeftButton || !flickable()) return event->ignore();
  m_buttonDown = true;
  m_startPoint = mapFromScreen(event->pos());
}

void DisplayItem::mouseMoveEvent(QMouseEvent* event) {
  if (!m_buttonDown) return event->ignore();

  QPointF p = mapFromScreen(event->pos());
  setLookAt(effectiveLookAt() - p + m_startPoint);
}

void DisplayItem::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton)
    m_buttonDown = false;
  else
    event->ignore();
}

void DisplayItem::touchEvent(QTouchEvent* event) {
  if (event->touchPointStates() & Qt::TouchPointPressed) {
    if (event->touchPoints().size() == 2) {
      QPointF p1 = mapFromScreen(event->touchPoints()[0].pos());
      QPointF p2 = mapFromScreen(event->touchPoints()[1].pos());

      m_distance = QVector2D(p1 - p2).length();
    }
  }

  if (event->touchPoints().size() == 1) {
    if (flickable()) {
      QPointF p1 = mapFromScreen(event->touchPoints().front().lastPos());
      QPointF p2 = mapFromScreen(event->touchPoints().front().pos());

      setLookAt(effectiveLookAt() - p2 + p1);
    } else
      event->ignore();
  } else if (event->touchPoints().size() == 2) {
    QPointF p1 = mapFromScreen(event->touchPoints()[0].pos());
    QPointF p2 = mapFromScreen(event->touchPoints()[1].pos());

    qreal distance = QVector2D(p1 - p2).length();

    setFactor(factor() * distance / m_distance);
  } else
    event->ignore();
}

void DisplayItem::visibleAreaChanged() {}

void DisplayItemFrame::sizeChanged() {}

DisplayItemFrame::DisplayItemFrame(Item* parent) : SceneGraph::Item(parent) {}

DisplayItemFrame::~DisplayItemFrame() {
  if (m_displayItem) m_displayItem->m_frame = nullptr;
}

void DisplayItemFrame::setSize(QSizeF s) {
  if (m_size != s) {
    m_size = s;
    if (m_displayItem) m_displayItem->updateVisibleArea();
    sizeChanged();
  }
}
