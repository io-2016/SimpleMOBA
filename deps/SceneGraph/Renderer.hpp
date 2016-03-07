#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <QMatrix4x4>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class QOpenGLTexture;

namespace SceneGraph {

class Node;
class GeometryNode;
class Item;
class Window;

class RenderState {
 private:
  friend class Renderer;

  QMatrix4x4 m_matrix;

  inline void setMatrix(QMatrix4x4 m) { m_matrix = m; }

 public:
  RenderState(QMatrix4x4 = QMatrix4x4());

  inline const QMatrix4x4& matrix() const { return m_matrix; }
};

class Renderer {
 private:
  friend class Node;

  Node* m_root;
  RenderState m_state;
  QSize m_size;
  uint m_frame;
  std::unordered_map<std::string, QOpenGLTexture*> m_texture;
  std::unordered_set<Node*> m_preprocess;

  void updateItem(Item*);
  void updateNodes(Window*);
  void destroyNodes(Window*);

  void nodeAdded(Node*);
  void nodeDestroyed(Node*);

 protected:
  virtual void renderGeometryNode(GeometryNode* node, const RenderState&) = 0;

 public:
  Renderer();
  virtual ~Renderer();

  virtual void render();
  void render(Node*, RenderState);

  void synchronize(Window* window);

  void setSize(QSize);
  inline QSize size() const { return m_size; }

  void setRoot(Item*);

  inline Node* root() const { return m_root; }

  QOpenGLTexture* texture(const char* path);

  inline uint frame() const { return m_frame; }
};
}

#endif  // RENDERER_HPP
