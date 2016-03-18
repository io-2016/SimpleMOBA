#ifndef ADDCIRCLE_HPP
#define ADDCIRCLE_HPP
#include "AddFixture.hpp"
#include "Graphics/Primitives.hpp"
#include "SceneGraph/Node.hpp"

class AddBody;

class AddCircle : public AddFixture {
 private:
  enum StateChange {
    PositionSet = 1 << 0,
    RadiusSet = 1 << 1,
    RadiusChanged = 1 << 2,
    ResetAction = 1 << 3
  };

  class Circle : public SceneGraph::TransformNode {
   public:
    CircleNode m_circleNode;

    Circle();
  };

  unsigned m_stateChange, m_state;
  QPointF m_position;
  float m_radius;
  ActionObject m_object;

 protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *);
  SceneGraph::Node *synchronize(SceneGraph::Node *old);

 public:
  AddCircle(AddBody *);

  QFixture *fixture() const;
  void reset();

  inline QString name() const { return "AddCircle"; }
};

#endif  // ADDCIRCLE_HPP
