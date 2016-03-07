#ifndef ADDPOLYGON_HPP
#define ADDPOLYGON_HPP
#include <vector>
#include "AddFixture.hpp"
#include "SceneGraph/Node.hpp"

class AddBody;

class AddPolygon : public AddFixture {
 private:
  ActionObject m_object;

  std::vector<QPointF> m_pts;

  class Node : public SceneGraph::Node {
   private:
    std::vector<SceneGraph::Node *> m_node;

   public:
    Node(std::vector<QPointF> pts, QSizeF size);
    ~Node();
  };

 protected:
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  SceneGraph::Node *synchronize(SceneGraph::Node *);

 public:
  AddPolygon(AddBody *);

  void reset();
  QFixture *fixture() const;

  inline QString name() const { return "AddPolygon"; }
};

#endif  // ADDPOLYGON_HPP
