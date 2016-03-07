#ifndef NODE_HPP
#define NODE_HPP
#include <QMatrix4x4>
#include "BaseObject.hpp"

namespace SceneGraph {

class Renderer;
class Material;
class Geometry;

class Node : protected BaseObject {
 public:
  enum class Type { None, GeometryNode, TransformNode };

  enum Flag { UsePreprocess = 1 << 0 };

 private:
  friend class Renderer;

  Renderer* m_renderer;
  Type m_type;
  Flag m_flag;

  void setRenderer(Renderer*);

 protected:
  virtual void preprocess();

 public:
  Node(Node* parent = nullptr, Type type = Type::None);
  ~Node();

  Node* firstChild() const;
  Node* next() const;
  Node* parent() const;
  void appendChild(Node*);
  void removeChild(Node*);

  inline Type type() const { return m_type; }
  inline Renderer* renderer() const { return m_renderer; }

  inline Flag flag() const { return m_flag; }
  void setFlag(Flag f);
};

class GeometryNode : public Node {
 private:
  Material* m_material;
  Geometry* m_geometry;

 public:
  GeometryNode(Node* parent = nullptr);

  inline void setMaterial(Material* m) { m_material = m; }
  inline Material* material() const { return m_material; }

  inline void setGeometry(Geometry* g) { m_geometry = g; }
  inline Geometry* geometry() const { return m_geometry; }
};

class TransformNode : public Node {
 private:
  QMatrix4x4 m_matrix;

 public:
  TransformNode(Node* parent = nullptr);

  inline const QMatrix4x4& matrix() const { return m_matrix; }
  inline void setMatrix(const QMatrix4x4& m) { m_matrix = m; }
};
}

#endif  // NODE_HPP
