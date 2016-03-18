#ifndef DYNAMICLIGHT_HPP
#define DYNAMICLIGHT_HPP

#include "Light.hpp"
#include "SceneGraph/Node.hpp"

class World;
class ViewWorld;
class StaticLight;

class ShadowNode : public SceneGraph::GeometryNode {
 private:
  struct Vertex {
    float x, y, z, w;
    Vertex(float, float, float, float);
  };

  SceneGraph::Geometry m_geometry;
  SceneGraph::ColorMaterial m_material;

 public:
  ShadowNode(QPointF p1, QPointF p2);

  void setVertices(QPointF p1, QPointF p2);
  void setColor(QColor);
};

class DynamicLight : public Light {
 private:
  StaticLight* m_boundLight;

  class DynamicNode : public Light::LightNode {
   private:
    SceneGraph::Node m_shadows;
    std::vector<ShadowNode*> m_unused;

   public:
    DynamicNode();
    ~DynamicNode();

    void synchronize(DynamicLight*, QWorld* world);
    void makeShadowNode(DynamicLight* light, QPointF p1, QPointF p2);
  };

  bool castingShadow(QPointF p1, QPointF p2) const;

 protected:
  SceneGraph::Node* synchronize(SceneGraph::Node*);

 public:
  explicit DynamicLight(SceneGraph::Item* = nullptr);
  ~DynamicLight();

  void bindLight(StaticLight* light);
};

#endif  // DYNAMICLIGHT_HPP
