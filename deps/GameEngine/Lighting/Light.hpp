#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "QBox2D/QBody.hpp"
#include "LightMaterial.hpp"
#include "SceneGraph/Node.hpp"
#include "SceneGraph/Geometry.hpp"

class LightSystem;

class Light : public QBody {
 private:
  QVector3D m_attenuation;
  qreal m_radius;
  qreal m_renderFraction;
  qreal m_z;
  QColor m_color;
  LightSystem* m_lightSystem;

 protected:
  class LightNode : public SceneGraph::GeometryNode {
   private:
    struct Vertex {
      float x, y;
    };

    SceneGraph::Geometry m_geometry;
    LightMaterial m_material;

   public:
    LightNode();

    inline LightMaterial* material() { return &m_material; }
    inline SceneGraph::Geometry* geometry() { return &m_geometry; }

    void updateGeometry(float radius);
    void synchronize(Light* light);
  };

  SceneGraph::Node* synchronize(SceneGraph::Node*);

 public:
  Light(Item* = nullptr);

  inline QVector3D attenuation() const { return m_attenuation; }
  void setAttenuation(QVector3D);

  inline qreal radius() const { return m_radius; }
  void setRadius(qreal);

  inline qreal renderFraction() const { return m_renderFraction; }
  void setRenderFraction(qreal);

  inline qreal z() const { return m_z; }
  void setZ(qreal);

  inline QColor color() const { return m_color; }
  void setColor(QColor);

  inline LightSystem* lightSystem() const { return m_lightSystem; }
  inline void setLightSystem(LightSystem* l) { m_lightSystem = l; }

  QRectF boundingRect() const;
  QRectF renderRect() const;

  bool read(const QJsonObject&);
  bool write(QJsonObject&) const;
};

#endif  // LIGHT_HPP
