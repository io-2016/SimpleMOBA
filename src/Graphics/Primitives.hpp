#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP
#include "SceneGraph/Geometry.hpp"
#include "SceneGraph/Node.hpp"
#include "SceneGraph/Material.hpp"
#include <QPointF>
#include <vector>

class Circle;

class ConvexPolygonGeometry : public SceneGraph::Geometry {
 public:
  ConvexPolygonGeometry(const std::vector<QPointF>& pts);
};

class ConvexPolygonNode : public SceneGraph::GeometryNode {
 private:
  ConvexPolygonGeometry m_geometry;
  SceneGraph::ColorMaterial m_material;

 public:
  ConvexPolygonNode(const std::vector<QPointF>& pts);
  void setColor(QColor);
};

class PolygonGeometry : public SceneGraph::Geometry {
 public:
  PolygonGeometry(const std::vector<QPointF>& pts);
};

class PolygonNode : public SceneGraph::GeometryNode {
 private:
  PolygonGeometry m_geometry;
  SceneGraph::ColorMaterial m_material;

 public:
  PolygonNode(const std::vector<QPointF>& pts);

  void setColor(QColor color);
};

class EdgeNode : public SceneGraph::GeometryNode {
 private:
  SceneGraph::Geometry m_geometry;
  SceneGraph::ColorMaterial m_material;

 public:
  EdgeNode();
  EdgeNode(QPointF p1, QPointF p2);

  void setP1(QPointF p1);
  void setP2(QPointF p2);

  void setColor(QColor color);
};

class CircleNode : public SceneGraph::GeometryNode {
 private:
  ConvexPolygonGeometry* m_geometry;
  SceneGraph::ColorMaterial m_material;

 public:
  CircleNode(QPointF pos, float radius, int accuracy = 32);
  CircleNode(const Circle& circle, int accuracy = 32);
  ~CircleNode();

  void setColor(QColor color);
};

#endif  // PRIMITIVES_HPP
