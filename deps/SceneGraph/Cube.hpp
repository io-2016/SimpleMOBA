#ifndef CUBE_HPP
#define CUBE_HPP
#include "Item.hpp"
#include "Node.hpp"
#include "Geometry.hpp"
#include "Material.hpp"

namespace SceneGraph {

class CubeGeometry : public Geometry {
 private:
  struct ColorVertex {
    float x, y, z, r, g, b, a;
  };

 public:
  CubeGeometry();
};

class Cube : public Item {
 private:
  class CubeNode : public GeometryNode {
   private:
    CubeGeometry m_geometry;
    VertexColorMaterial m_material;

   public:
    CubeNode(Node* = nullptr);
  };

 protected:
  Node* synchronize(Node* old);

 public:
  Cube(Item* = nullptr);
};
}

#endif  // CUBE_HPP
