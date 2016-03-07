#ifndef BOX2DCHAIN_HPP
#define BOX2DCHAIN_HPP
#include "Box2DPolygon.hpp"

class Box2DChain : public Box2DPolygon {
 private:
 protected:
  b2Shape* createShape() const;
  SceneGraph::Node* synchronize(SceneGraph::Node*);

 public:
  explicit Box2DChain(Item* = nullptr);

  bool write(QJsonObject&) const;
};

#endif  // BOX2DCHAIN_HPP
