#ifndef ENLIGHTEDITEMS_HPP
#define ENLIGHTEDITEMS_HPP

#include "LightMaterial.hpp"
#include "SceneGraph/Geometry.hpp"
#include "SceneGraph/Item.hpp"
#include "Utility/Utility.hpp"
#include <unordered_map>

class LightSystem;
class QFixture;
class Light;
class QWorld;

class EnlightedNode : public SceneGraph::TransformNode {
private:
  SceneGraph::GeometryNode m_geometryNode;
  LightMaterial m_material;

  void updateMaterial(Light *);
  void updateMatrix(QFixture *);

public:
  EnlightedNode();

  void update(QFixture *, Light *);
  void setGeometry(SceneGraph::Geometry *);
};

class EnlightedItems : public SceneGraph::Item {
private:
  friend class LightSystem;

  LightSystem *m_lightSystem;
  uint m_state;
  std::vector<void *> m_destroyedFixture;

  enum State {
    Reset = 1 << 0,
  };

protected:
  class Node : public SceneGraph::Node {
  private:
    struct Vertex {
      float x, y;
    };

    std::unordered_map<void *, SceneGraph::Geometry *> m_data;
    std::vector<EnlightedNode *> m_node;

  public:
    Node();
    ~Node();

    void clear();
    SceneGraph::Geometry *geometry(QFixture *);

    EnlightedNode *getNode(QFixture *f, Light *light, uint it);
    void destroyedFixture(void *f);
  };

  SceneGraph::Node *synchronize(SceneGraph::Node *old);
  void onFixtureDestroyed(QFixture *);

public:
  explicit EnlightedItems(LightSystem *, SceneGraph::Item *);

  void clear();

  inline LightSystem *lightSystem() const { return m_lightSystem; }
  QWorld *world() const;
};

#endif // ENLIGHTEDITEMS_HPP
