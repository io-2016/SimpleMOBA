#ifndef LIGHTSYSTEM_HPP
#define LIGHTSYSTEM_HPP

#include <unordered_set>
#include "SceneGraph/ShaderSource.hpp"
#include "EnlightedItems.hpp"
#include "LightBlender.hpp"
#include "DynamicLight.hpp"

class StaticLight;
class DisplayItem;
class QWorld;
class Game;

class LightSystem : public SceneGraph::Item {
 private:
  friend class StaticLight;
  friend class DynamicLight;
  friend class World;
  friend class ViewWorld;

  QWorld* m_world;
  QSizeF m_size;
  QSize m_resolution;

  SceneGraph::ShaderSource m_normalMap;
  SceneGraph::ShaderSource m_lightTexture;
  SceneGraph::ShaderSource m_framebuffer[DYNAMIC_LIGHTS_COUNT];
  DynamicLight m_dynamicLight[DYNAMIC_LIGHTS_COUNT];

  EnlightedItems m_enlightedItems;

  std::vector<DynamicLight*> m_unusedLight;
  std::vector<StaticLight*> m_light;
  std::unordered_set<StaticLight*> m_visibleLights;
  std::unordered_set<StaticLight*> m_loadedLights;

  void addLight(StaticLight*);
  void removeLight(StaticLight*);

  void initializeDynamicLights();

  void lightVisibilityChanged(StaticLight*);
  void visibleAreaChanged(QRectF);

  void addBody(QBody*);

 protected:
  SceneGraph::Node* synchronize(SceneGraph::Node* old);
  void onFixtureDestroyed(QFixture*);

 public:
  explicit LightSystem(SceneGraph::Item* parent, QWorld*);
  ~LightSystem();

  void initialize();
  void clear();

  inline QSizeF size() const { return m_size; }
  void setSize(QSizeF);

  inline QSize resolution() const { return m_resolution; }
  void setResolution(QSize s);

  inline SceneGraph::ShaderSource* normalMap() { return &m_normalMap; }
  inline SceneGraph::ShaderSource* lightTexture() { return &m_lightTexture; }

  QWorld* world() const;

  const std::unordered_set<StaticLight*>& visibleLights() const;

  void read(const QJsonObject&);
  void write(QJsonObject&) const;
};

#endif  // LIGHTSYSTEM_HPP
