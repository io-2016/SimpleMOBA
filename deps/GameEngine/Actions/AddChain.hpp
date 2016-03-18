#ifndef ADDCHAIN_HPP
#define ADDCHAIN_HPP
#include "SubAction.hpp"
#include "SceneGraph/Node.hpp"
#include "SceneGraph/Material.hpp"
#include "SceneGraph/Geometry.hpp"

class AddChain : public SubAction {
 private:
  class Node : public SceneGraph::GeometryNode {
   private:
    SceneGraph::ColorMaterial m_material;
    SceneGraph::Geometry m_geometry;

   public:
    Node(std::vector<QPointF> pts);

    void setLastPoint(QPointF);
  };

  enum DirtyState { MousePos = 1 << 0, Points = 1 << 1, Finished = 1 << 2 };

  typedef int State;

  std::vector<QPointF> m_pts;
  QPointF m_mousePos;
  State m_state;
  ActionObject m_object;

 protected:
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent*);
  void keyPressEvent(QKeyEvent* event);
  SceneGraph::Node* synchronize(SceneGraph::Node*);

 public:
  AddChain(MapEditor* = nullptr);

  inline const std::vector<QPointF>& pts() const { return m_pts; }

  void reset();

  inline QString name() const { return "AddChain"; }
};

#endif  // ADDCHAIN_HPP
