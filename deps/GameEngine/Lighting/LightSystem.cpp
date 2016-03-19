#include "LightSystem.hpp"
#include "StaticLight.hpp"
#include "QBox2D/QWorld.hpp"
#include "Utility/Utility.hpp"
#include <QJsonArray>
#include <QJsonObject>
#include <cassert>

LightSystem::LightSystem(SceneGraph::Item* parent, QWorld* world)
    : SceneGraph::Item(parent),
      m_world(world),
      m_resolution(),
      m_normalMap(this),
      m_lightTexture(this),
      m_enlightedItems(this, lightTexture()->sourceItem()) {}

LightSystem::~LightSystem() {
  clear();

  for (StaticLight* light : m_light) {
    light->setVisible(false);
    light->setLightSystem(nullptr);
  }

  for (DynamicLight* light : m_unusedLight) {
    light->setVisible(false);
    light->setLightSystem(nullptr);
  }
}

void LightSystem::read(const QJsonObject& obj) {
  QJsonArray array = obj["lights"].toArray();

  for (int i = 0; i < array.size(); i++) {
    QJsonObject lightData = array[i].toObject();

    StaticLight* light = new StaticLight(world());
    light->setLightSystem(this);
    light->read(lightData);
    light->initialize(world());

    m_loadedLights.insert(light);
  }
}

void LightSystem::write(QJsonObject& obj) const {
  QJsonArray array;
  for (StaticLight* light : m_loadedLights) {
    QJsonObject object;
    if (light->write(object)) {
      array.append(object);
    }
  }

  obj["lights"] = array;
}

const std::unordered_set<StaticLight*>& LightSystem::visibleLights() const {
  return m_visibleLights;
}

void LightSystem::initialize() {
  initializeDynamicLights();

  QColor color;
  color.setRgbF(0.5, 0.5, 1.0);
  m_normalMap.setBackground(color);

  visibleAreaChanged(world()->visibleRect());
}

void LightSystem::clear() {
  while (!m_loadedLights.empty()) delete *m_loadedLights.begin();

  m_enlightedItems.clear();
}

void LightSystem::setSize(QSizeF s) {
  m_size = s;

  resetTransform();
  scale(m_size.width(), m_size.height());
}

void LightSystem::setResolution(QSize s) {
  m_resolution = s;

  for (SceneGraph::ShaderSource& fbo : m_framebuffer)
    fbo.setTextureSize(m_resolution);
  normalMap()->setTextureSize(m_resolution);
  lightTexture()->setTextureSize(m_resolution);
}

QWorld* LightSystem::world() const {
  return m_world;
}

void LightSystem::addLight(StaticLight* light) {
  m_light.push_back(light);
  light->setParent(nullptr);
}

void LightSystem::removeLight(StaticLight* light) {
  light->setVisible(false);
  light->setLightSystem(nullptr);

  auto it1 = std::find(m_light.begin(), m_light.end(), light);
  assert(it1 != m_light.end());
  m_light.erase(it1);

  auto it2 = m_loadedLights.find(light);
  if (it2 != m_loadedLights.end()) m_loadedLights.erase(it2);
}

void LightSystem::lightVisibilityChanged(StaticLight* light) {
  if (light->visible())
    m_visibleLights.insert(light);
  else
    m_visibleLights.erase(m_visibleLights.find(light));
  if (light->dynamicLight()) {
    if (!light->visible()) {
      DynamicLight* p = light->dynamicLight();
      p->setVisible(false);
      m_unusedLight.push_back(p);

      p->bindLight(nullptr);
    }
  } else {
    if (light->visible() && light->dynamicShadows() && !m_unusedLight.empty()) {
      DynamicLight* p = m_unusedLight.back();
      m_unusedLight.pop_back();

      p->setVisible(true);
      p->bindLight(light);

      light->setParent(nullptr);
    } else if (light->visible()) {
      light->setParent(lightTexture()->sourceItem());
    }
  }
}

void LightSystem::visibleAreaChanged(QRectF rect) {
  for (SceneGraph::ShaderSource& i : m_framebuffer) i.setSourceRect(rect);

  normalMap()->setSourceRect(rect);
  lightTexture()->setSourceRect(rect);
}

void LightSystem::addBody(QBody* body) {
  body->content()->setParent(normalMap()->sourceItem());
}

SceneGraph::Node* LightSystem::synchronize(SceneGraph::Node* old) {
  LightBlender* node = static_cast<LightBlender*>(old);
  if (!node) {
    node = new LightBlender;

    SceneGraph::ShaderSource* array[DYNAMIC_LIGHTS_COUNT];
    for (uint i = 0; i < DYNAMIC_LIGHTS_COUNT; i++)
      array[i] = &m_framebuffer[i];

    node->material()->setLights(array);
    node->material()->setNormalMap(normalMap());
    node->material()->setAmbient(QColor(20, 20, 20, 0));
    node->material()->setLightTexture(lightTexture());
  }

  return node;
}

void LightSystem::onFixtureDestroyed(QFixture* f) {
  m_enlightedItems.onFixtureDestroyed(f);
}

void LightSystem::initializeDynamicLights() {
  for (uint i = 0; i < DYNAMIC_LIGHTS_COUNT; i++) {
    m_framebuffer[i].setParent(this);

    m_dynamicLight[i].setParent(m_framebuffer[i].sourceItem());
    m_dynamicLight[i].setLightSystem(this);
    m_dynamicLight[i].setVisible(false);
    m_dynamicLight[i].initialize(world());

    m_unusedLight.push_back(&m_dynamicLight[i]);
  }
}
