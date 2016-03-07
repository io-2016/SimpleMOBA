#ifndef TEXTUREDCONVEXPOLYGON_HPP
#define TEXTUREDCONVEXPOLYGON_HPP
#include "TexturedPolygon.hpp"

class TexturedConvexPolygon : public TexturedPolygon {
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
  explicit TexturedConvexPolygon(Item* = nullptr);
};

#endif  // TEXTUREDCONVEXPOLYGON_HPP
