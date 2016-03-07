#ifndef TEXTUREDRECTANGLE_HPP
#define TEXTUREDRECTANGLE_HPP

#include "TexturedItem.hpp"
#include "SceneGraph/Node.hpp"
#include "SceneGraph/Geometry.hpp"
#include "SceneGraph/Material.hpp"

class TexturedRectangle : public TexturedItem {
 private:
  QRectF m_textureRect;

 protected:
  class Node : public SceneGraph::Node {
   private:
    SceneGraph::GeometryNode m_geometryNode;
    SceneGraph::Geometry m_geometry;
    SceneGraph::TextureMaterial m_material;

   public:
    Node();

    void synchronize(const TexturedRectangle*);

    void updateMaterial(const TexturedRectangle*);
    void updateGeometry(const TexturedRectangle*);
  };

  SceneGraph::Node* synchronize(SceneGraph::Node* old);

 public:
  explicit TexturedRectangle(SceneGraph::Item* = nullptr);

  inline QRectF textureRect() const { return m_textureRect; }
  void setTextureRect(QRectF);
};

#endif  // TEXTUREDRECTANGLE_HPP
