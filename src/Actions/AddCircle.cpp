#include "AddCircle.hpp"
#include "AddBody.hpp"
#include "Graphics/Primitives.hpp"
#include "Geometry/Vector2d.hpp"
#include "QBox2D/Fixture/Box2DCircle.hpp"
#include "QBox2D/QWorld.hpp"
#include <QMouseEvent>

AddCircle::AddCircle(AddBody* parent)
    : AddFixture(parent), m_stateChange(), m_state(), m_object(this) {}

QFixture* AddCircle::fixture() const {
  Box2DCircle* circle = new Box2DCircle;
  circle->setPosition(m_position);
  circle->setRadius(m_radius);

  return circle;
}

void AddCircle::reset() {
  m_stateChange = ResetAction;
  m_state = 0;
  m_radius = 0;
  update();
}

void AddCircle::mousePressEvent(QMouseEvent*) {}

void AddCircle::mouseReleaseEvent(QMouseEvent* event) {
  if (event->button() == Qt::LeftButton) {
    if (!(m_state & PositionSet)) {
      m_position = mapFromScreen(event->pos());
      m_stateChange |= PositionSet;
      m_state |= PositionSet;
    } else {
      m_stateChange |= RadiusSet;
      m_state |= RadiusSet;
      finished();
    }

    update();
  }
}

void AddCircle::mouseMoveEvent(QMouseEvent* event) {
  if ((m_state & PositionSet) && !(m_state & RadiusSet)) {
    m_radius = Vector2d(m_position - mapFromScreen(event->pos())).length();
    m_stateChange |= RadiusChanged;
    update();
  }
}

SceneGraph::Node* AddCircle::synchronize(SceneGraph::Node* old) {
  Circle* node = static_cast<Circle*>(old);

  if (m_stateChange & ResetAction) {
    m_stateChange ^= ResetAction;
    node = nullptr;
  } else if (m_stateChange) {
    m_stateChange = 0;

    QMatrix4x4 matrix;
    matrix.translate(m_position.x(), m_position.y());
    matrix.scale(m_radius);

    node = new Circle;
    node->setMatrix(matrix);
    node->m_circleNode.setColor(Qt::yellow);
  }

  return node;
}

AddCircle::Circle::Circle() : m_circleNode(QPointF(0, 0), 1) {
  appendChild(&m_circleNode);
}
