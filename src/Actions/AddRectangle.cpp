#include "AddRectangle.hpp"
#include "AddBody.hpp"
#include "QBox2D/Fixture/Box2DBox.hpp"
#include "Graphics/Primitives.hpp"

AddRectangle::AddRectangle(AddBody *p)
    : AddFixture(p), m_state(), m_object(this) {}

void AddRectangle::reset() {
  m_state = Reset;

  update();
}

QFixture *AddRectangle::fixture() const {
  Box2DBox *box = new Box2DBox;
  box->setPosition(
      QPointF(std::min(m_p1.x(), m_p2.x()), std::min(m_p1.y(), m_p2.y())));

  box->setSize(
      QSizeF(std::abs(m_p1.x() - m_p2.x()), std::abs(m_p1.y() - m_p2.y())));

  box->setTextureSource(":/resources/crate.jpg");

  return box;
}

void AddRectangle::mousePressEvent(QMouseEvent *) {}

void AddRectangle::mouseReleaseEvent(QMouseEvent *event) {
  if (!(m_state & SetFirst)) {
    m_p1 = mapFromScreen(event->pos());
    m_state |= SetFirst;
  } else if (!(m_state & SetSecond)) {
    m_p2 = mapFromScreen(event->pos());
    m_state |= SetSecond;
    finished();
  }
}

void AddRectangle::mouseMoveEvent(QMouseEvent *event) {
  if ((m_state & SetFirst) && !(m_state & SetSecond)) {
    m_cursor = mapFromScreen(event->pos());
    m_state |= MovedCursor;

    update();
  }
}

SceneGraph::Node *AddRectangle::synchronize(SceneGraph::Node *old) {
  SceneGraph::TransformNode *node =
      static_cast<SceneGraph::TransformNode *>(old);

  if (m_state & Reset) {
    m_state ^= Reset;
    node = nullptr;
  }

  if (m_state & MovedCursor) {
    m_state ^= MovedCursor;
    QMatrix4x4 matrix;
    matrix.translate(m_p1.x(), m_p1.y());
    matrix.scale(m_cursor.x() - m_p1.x(), m_cursor.y() - m_p1.y());

    if (!node) node = new Rectangle;
    node->setMatrix(matrix);
  }

  return node;
}

AddRectangle::Rectangle::Rectangle()
    : m_geometryNode(
          {QPointF(0, 0), QPointF(0, 1), QPointF(1, 1), QPointF(1, 0)}) {
  appendChild(&m_geometryNode);
  m_geometryNode.setColor(Qt::red);
}

AddRectangle::Rectangle::~Rectangle() {}
