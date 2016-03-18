#ifndef ADDRECTANGLE_HPP
#define ADDRECTANGLE_HPP
#include "AddFixture.hpp"
#include "SceneGraph/Node.hpp"
#include "Graphics/Primitives.hpp"

class AddBody;

class AddRectangle : public AddFixture {
 private:
  enum State {
    SetFirst = 1 << 0,
    SetSecond = 1 << 1,
    MovedCursor = 1 << 2,
    Reset = 1 << 3
  };

  unsigned m_state;
  QPointF m_p1, m_p2, m_cursor;
  ActionObject m_object;

  class Rectangle : public SceneGraph::TransformNode {
   private:
    ConvexPolygonNode m_geometryNode;

   public:
    Rectangle();
    ~Rectangle();
  };

 protected:
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent*);

  SceneGraph::Node* synchronize(SceneGraph::Node*);

 public:
  AddRectangle(AddBody*);

  void reset();
  QFixture* fixture() const;

  inline QString name() const { return "AddRectangle"; }
};

#endif  // ADDRECTANGLE_HPP
