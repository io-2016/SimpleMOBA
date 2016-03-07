#include "Cube.hpp"

namespace SceneGraph {

Cube::Cube(Item* parent) : Item(parent) {}

Node* Cube::synchronize(Node* old) {
  CubeNode* node = static_cast<CubeNode*>(old);

  if (!node) {
    node = new CubeNode;
  }

  return node;
}

Cube::CubeNode::CubeNode(Node* parent) : GeometryNode(parent) {
  setGeometry(&m_geometry);
  setMaterial(&m_material);
}

CubeGeometry::CubeGeometry()
    : Geometry({{3, GL_FLOAT}, {4, GL_FLOAT}}, 24, sizeof(ColorVertex), 34) {
	std::vector<ColorVertex> vertex = {};/*{
      {-1, -1, 1, 0.3, 0.3, 0, 1}, {1, -1, 1, 0.3, 0.3, 0, 1},
      {-1, 1, 1, 0.3, 0.3, 0, 1},  {1, 1, 1, 0.3, 0.3, 0, 1},

      {1, -1, 1, 1, 0, 1, 1},      {1, -1, -1, 1, 0, 1, 1},
      {1, 1, 1, 1, 0, 1, 1},       {1, 1, -1, 1, 0, 1, 1},

      {1, -1, -1, 1, 0, 0, 1},     {-1, -1, -1, 1, 0, 0, 1},
      {1, 1, -1, 1, 0, 0, 1},      {-1, 1, -1, 1, 0, 0, 1},

      {-1, -1, -1, 0, 1, 0, 1},    {-1, -1, 1, 0, 1, 0, 1},
      {-1, 1, -1, 0, 1, 0, 1},     {-1, 1, 1, 0, 1, 0, 1},

      {-1, -1, -1, 0, 0, 1, 1},    {1, -1, -1, 0, 0, 1, 1},
      {-1, -1, 1, 0, 0, 1, 1},     {1, -1, 1, 0, 0, 1, 1},

      {-1, 1, 1, 0, 1, 1, 1},      {1, 1, 1, 0, 1, 1, 1},
      {-1, 1, -1, 0, 1, 1, 1},     {1, 1, -1, 0, 1, 1, 1}};*/
  memcpy(vertexData(), vertex.data(), vertex.size() * sizeof(ColorVertex));
  updateVertexData();

  std::vector<uint> index = {0,  1,  2,  3,  3,  4,  4,  5,  6,  7,  7,  8,
                             8,  9,  10, 11, 11, 12, 12, 13, 14, 15, 15, 16,
                             16, 17, 18, 19, 19, 20, 20, 21, 22, 23};

  memcpy(indexData(), index.data(), index.size() * sizeof(uint));
}
}
