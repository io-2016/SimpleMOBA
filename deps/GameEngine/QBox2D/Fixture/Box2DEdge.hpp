#ifndef BOX2DEDGE_HPP
#define BOX2DEDGE_HPP

#include "QBox2D/QFixture.hpp"

class Box2DEdge : public QFixture {
 private:
  b2EdgeShape m_edgeShape;

 protected:
  b2Shape* createShape() const;
  SceneGraph::Node* synchronize(SceneGraph::Node*);

 public:
  explicit Box2DEdge(SceneGraph::Item* parent = nullptr);

  inline b2EdgeShape& edgeShape() { return m_edgeShape; }
  std::vector<QPointF> vertices() const;
};

#endif  // BOX2DEDGE_HPP
