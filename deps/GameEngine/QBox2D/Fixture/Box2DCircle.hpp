#ifndef BOX2DCIRCLE_HPP
#define BOX2DCIRCLE_HPP

#include "QBox2D/QFixture.hpp"

class Box2DCircle : public QFixture {
 private:
  float m_radius;

 protected:
  b2Shape* createShape() const;
  SceneGraph::Node* synchronize(SceneGraph::Node*);

 public:
  explicit Box2DCircle(SceneGraph::Item* = nullptr);

  inline float radius() const { return m_radius; }
  inline void setRadius(float radius) { m_radius = radius; }

  std::vector<QPointF> vertices() const;

  bool write(QJsonObject&) const;
};

#endif  // BOX2DCIRCLE_HPP
