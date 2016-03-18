#ifndef BOX2DPOLYGON_HPP
#define BOX2DPOLYGON_HPP
#include "Fixture.hpp"
#include "Graphics/TexturedConvexPolygon.hpp"

class Box2DPolygon : public Fixture {
 private:
  friend class Box2DChain;

  std::vector<QPointF> m_vertices;

  TexturedConvexPolygon m_texture;
  TexturedConvexPolygon m_normalMap;

 protected:
  b2Shape* createShape() const;
  SceneGraph::Node* synchronize(SceneGraph::Node*);

 public:
  explicit Box2DPolygon(Item* = nullptr);

  inline std::vector<QPointF> vertices() const { return m_vertices; }
  void setVertices(const std::vector<QPointF>& pts);

  inline QVector2D textureScale() const { return m_texture.textureScale(); }
  void setTextureScale(QVector2D);

  bool read(const QJsonObject&);
  bool write(QJsonObject&) const;
};

#endif  // BOX2DPOLYGON_HPP
