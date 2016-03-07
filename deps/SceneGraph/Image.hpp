#ifndef IMAGE_HPP
#define IMAGE_HPP
#include "Item.hpp"
#include "Node.hpp"
#include "Material.hpp"
#include "Geometry.hpp"
#include <string>

namespace SceneGraph {

class Image : public Item {
 private:
  class ImageNode : public GeometryNode {
   private:
    Geometry m_geometry;
    TextureMaterial m_material;

    struct Vertex {
      float x, y, tx, ty;
    };

   public:
    ImageNode(Node* parent = nullptr);

    void setTexture(QOpenGLTexture*);
  };

  std::string m_source;

 protected:
  Node* synchronize(Node* old);

 public:
  Image(Item* = nullptr);

  inline const std::string& source() const { return m_source; }
  inline void setSource(const std::string& p) { m_source = p; }
};
}

#endif  // IMAGE_HPP
