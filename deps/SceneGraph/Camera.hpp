#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "Item.hpp"

namespace SceneGraph {

class Camera : public Item {
 private:
  QVector3D m_position;
  QVector3D m_direction;
  QVector3D m_right;
  float m_verticalAngle;
  float m_horizontalAngle;

  void updateMatrix();

 protected:
  void keyPressEvent(QKeyEvent *);
  void mouseMoveEvent(QMouseEvent *);

 public:
  Camera(Item * = nullptr);
};
}

#endif  // PLAYER_HPP
