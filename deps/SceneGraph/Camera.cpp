#include "Camera.hpp"
#include "Window.hpp"
#include "Geometry.hpp"
#include <QKeyEvent>
#include <QCursor>

namespace SceneGraph {

Camera::Camera(Item* parent)
    : Item(parent),
      m_position(0, 0, 5),
      m_direction(0, 0, 0),
      m_verticalAngle(-M_PI),
      m_horizontalAngle() {
  updateMatrix();
}

void Camera::updateMatrix() {
  m_direction = QVector3D(cos(m_verticalAngle) * sin(m_horizontalAngle),
                          sin(m_verticalAngle),
                          cos(m_verticalAngle) * cos(m_horizontalAngle))
                    .normalized();

  m_right = QVector3D(sin(m_horizontalAngle - M_PI / 2), 0,
                      cos(m_horizontalAngle - M_PI / 2))
                .normalized();

  QMatrix4x4 matrix;
  matrix.lookAt(m_position, m_position + m_direction,
                -QVector3D::crossProduct(m_right, m_direction));
  setMatrix(matrix);
}

void Camera::keyPressEvent(QKeyEvent* event) {
  const float MOVE = 0.1;

  if (event->key() == Qt::Key_W)
    m_position += MOVE * m_direction;
  else if (event->key() == Qt::Key_S)
    m_position -= MOVE * m_direction;
  else if (event->key() == Qt::Key_A)
    m_position += MOVE * m_right;
  else if (event->key() == Qt::Key_D)
    m_position -= MOVE * m_right;

  updateMatrix();
}

void Camera::mouseMoveEvent(QMouseEvent* event) {
  const float SPEED = 0.001;
  QPoint screenSize(window()->width(), window()->height());
  QPointF pos = window()->mapToGlobal(0.5 * screenSize);
  QCursor::setPos(pos.x(), pos.y());

  QPointF diff = event->pos() - 0.5 * screenSize;
  QVector2D vec = QVector2D(diff.x(), diff.y());

  m_verticalAngle += SPEED * vec.y();
  m_horizontalAngle -= SPEED * vec.x();

  updateMatrix();
}
}
