#include "EnlightedItems.hpp"
#include "LightSystem.hpp"
#include "Light.hpp"
#include "QBox2D/QWorld.hpp"
#include "QBox2D/QFixture.hpp"
#include "StaticLight.hpp"
#include <cassert>

EnlightedItems::EnlightedItems(LightSystem* system, SceneGraph::Item* parent)
    : SceneGraph::Item(parent), m_lightSystem(system), m_state() {
}

void EnlightedItems::clear() { m_state |= Reset; }

QWorld* EnlightedItems::world() const { return lightSystem()->world(); }

SceneGraph::Node* EnlightedItems::synchronize(SceneGraph::Node* old) {
  Node* node = static_cast<Node*>(old);
  if (!node) node = new Node;
  while (node->firstChild()) node->removeChild(node->firstChild());

  if (m_state & Reset) {
    m_state &= ~Reset;
    node->clear();
  }

  while (!m_destroyedFixture.empty()) {
    node->destroyedFixture(m_destroyedFixture.back());
    m_destroyedFixture.pop_back();
  }

  uint it = 0;
  QRectF visibleArea = world()->visibleRect();
  for (StaticLight* light : lightSystem()->visibleLights()) {
    if (!light->dynamicLight()) continue;

    QRectF lightRect = light->matrix().mapRect(light->boundingRect());
    QRectF rect = visibleArea.intersected(lightRect);
    for (QFixture* f : world()->fixtures(rect)) {
      if (f->shadowCaster()) {
        SceneGraph::Node* enlightedNode = node->getNode(f, light, it++);
        node->appendChild(enlightedNode);
      }
    }
  }

  update();

  return node;
}

void EnlightedItems::onFixtureDestroyed(QFixture* f) {
  if (f->shadowCaster()) m_destroyedFixture.push_back(f);
}

EnlightedNode::EnlightedNode() {
  m_geometryNode.setMaterial(&m_material);
  appendChild(&m_geometryNode);
}

void EnlightedNode::update(QFixture* fixture, Light* light) {
  updateMatrix(fixture);
  updateMaterial(light);
}

void EnlightedNode::setGeometry(SceneGraph::Geometry* g) {
  m_geometryNode.setGeometry(g);
}

void EnlightedNode::updateMaterial(Light* light) {
  m_material.setNormalMap(light->lightSystem()->normalMap()->shaderNode());
  QVector3D p(light->position().x(), light->position().y(), light->z());
  m_material.setLightPosition(matrix().inverted() * p);
  m_material.setColor(light->color());
  m_material.setAttenuation(light->attenuation());
}

void EnlightedNode::updateMatrix(QFixture* fixture) {
  setMatrix(fixture->body()->matrix() * fixture->matrix());
}

EnlightedItems::Node::Node() : m_node(1) {}

EnlightedItems::Node::~Node() { clear(); }

void EnlightedItems::Node::clear() {
  for (std::pair<void*, SceneGraph::Geometry*> pair : m_data)
    delete pair.second;
  m_data.clear();

  for (EnlightedNode* node : m_node) delete node;
  m_node.clear();
  m_node.resize(1);
}

SceneGraph::Geometry* EnlightedItems::Node::geometry(QFixture* fixture) {
  if (m_data.find(fixture) != m_data.end()) return m_data[fixture];

  std::vector<QPointF> vert = fixture->vertices();
  SceneGraph::Geometry* g = new SceneGraph::Geometry(
      {{2, GL_FLOAT}}, vert.size() - 1, sizeof(Vertex));
  Vertex* array = g->vertexData<Vertex>();
  g->setDrawingMode(GL_TRIANGLE_FAN);
  for (uint i = 0; i + 1 < vert.size(); i++)
    array[i] = {float(vert[i].x()), float(vert[i].y())};
  g->updateVertexData();

  return m_data[fixture] = g;
}

EnlightedNode* EnlightedItems::Node::getNode(QFixture* f, Light* light,
                                             uint it) {
  if (it >= m_node.size()) m_node.resize(2 * m_node.size());

  if (m_node[it] == nullptr) m_node[it] = new EnlightedNode;

  EnlightedNode* node = m_node[it];
  node->setGeometry(geometry(f));
  node->update(f, light);

  return node;
}

void EnlightedItems::Node::destroyedFixture(void* f) {
  auto it = m_data.find(f);

  if (it != m_data.end()) {
    delete it->second;
    m_data.erase(it);
  }
}
