#ifndef TEXTUREDPOLYGON_HPP
#define TEXTUREDPOLYGON_HPP

#include "SceneGraph/Geometry.hpp"
#include "SceneGraph/Node.hpp"
#include "SceneGraph/Material.hpp"
#include "TexturedItem.hpp"

class TexturedPolygon : public TexturedItem {
 private:
  QVector2D m_textureScale;
  std::vector<QPointF> m_vertices;

 protected:
  class Node : public SceneGraph::Node {
   private:
    SceneGraph::GeometryNode m_geometryNode;
    SceneGraph::Geometry m_geometry;
    SceneGraph::TextureMaterial m_material;

   public:
    Node(const std::vector<QPointF>& pts, QVector2D textureScale);

    void setTexture(QOpenGLTexture*);
  };

  SceneGraph::Node* synchronize(SceneGraph::Node*);

 public:
  explicit TexturedPolygon(Item* parent = nullptr);

  inline const std::vector<QPointF>& vertices() const { return m_vertices; }
  inline void setVertices(const std::vector<QPointF>& v) { m_vertices = v; }

  inline const QVector2D& textureScale() const { return m_textureScale; }
  inline void setTextureScale(const QVector2D& s) { m_textureScale = s; }
};

#endif  // TEXTUREDPOLYGON_HPP
