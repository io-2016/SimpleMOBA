#include "DynamicLight.hpp"
#include "LightSystem.hpp"
#include "QBox2D/QWorld.hpp"
#include "StaticLight.hpp"
#include <QQuickWindow>
#define GLSL(shader) #shader

ShadowNode::ShadowNode(QPointF p1, QPointF p2)
    : m_geometry({{4, GL_FLOAT}}, 4, sizeof(Vertex)) {
  setGeometry(&m_geometry);
  setMaterial(&m_material);

  m_geometry.setDrawingMode(GL_TRIANGLE_FAN);

  setVertices(p1, p2);
  setColor(Qt::black);
}

void ShadowNode::setVertices(QPointF p1, QPointF p2) {
  Vertex *array = m_geometry.vertexData<Vertex>();

  array[0] = Vertex(p1.x(), p1.y(), 0.0, 1.0);
  array[1] = Vertex(p2.x(), p2.y(), 0.0, 1.0);
  array[2] = Vertex(p2.x(), p2.y(), 0.0, 0.0);
  array[3] = Vertex(p1.x(), p1.y(), 0.0, 0.0);

  m_geometry.updateVertexData();
}

void ShadowNode::setColor(QColor color) { m_material.setColor(color); }

DynamicLight::DynamicLight(Item *item)
    : Light(item), m_boundLight() {}

DynamicLight::~DynamicLight() {
  if (m_boundLight) {
    m_boundLight->m_dynamicLight = nullptr;
    m_boundLight = nullptr;
  }
}

void DynamicLight::bindLight(StaticLight *light) {
  if (m_boundLight)
    m_boundLight->m_dynamicLight = nullptr;

  m_boundLight = light;

  if (!light) {
    setVisible(false);
    return;
  }

  setPosition(light->position());
  setRadius(light->radius());
  setColor(light->color());
  setAttenuation(light->attenuation());
  setRenderFraction(light->renderFraction());
  setZ(light->z());

  light->m_dynamicLight = this;
}

bool DynamicLight::castingShadow(QPointF p1, QPointF p2) const {
  return p1.x() * p2.y() - p1.y() * p2.x() > 0;
}

SceneGraph::Node *DynamicLight::synchronize(SceneGraph::Node *old) {
  DynamicNode *rootNode = static_cast<DynamicNode *>(old);
  if (!rootNode)
    rootNode = new DynamicNode;

  rootNode->synchronize(this, lightSystem()->world());

  return rootNode;
}

DynamicLight::DynamicNode::DynamicNode() { appendChild(&m_shadows); }

DynamicLight::DynamicNode::~DynamicNode() {
  for (ShadowNode *node : m_unused)
    delete node;
  m_unused.clear();

  while (m_shadows.firstChild())
    delete m_shadows.firstChild();
}

void DynamicLight::DynamicNode::synchronize(DynamicLight *light,
                                            QWorld *world) {
  LightNode::synchronize(light);

  while (m_shadows.firstChild()) {
    m_unused.push_back(static_cast<ShadowNode *>(m_shadows.firstChild()));
    m_shadows.removeChild(m_shadows.firstChild());
  }

  QRectF lightRect = light->matrix().mapRect(light->boundingRect());
  QRectF rect = world->visibleRect().intersected(lightRect);

  for (QFixture *fixture : world->fixtures(rect)) {
    if (!fixture->shadowCaster() || !fixture->body()->visible())
      continue;

    std::vector<QPointF> vertices;
    for (QPointF vert : fixture->vertices()) {
      QPointF p = fixture->body()->matrix() * fixture->matrix() * vert;
      vertices.push_back(light->matrix().inverted() * p);
    }

    for (uint i = 0; i + 1 < vertices.size(); i++) {
      makeShadowNode(light, vertices[i], vertices[i + 1]);
    }
  }
}

void DynamicLight::DynamicNode::makeShadowNode(DynamicLight *light, QPointF p1,
                                               QPointF p2) {
  if (!light->castingShadow(p1, p2))
    return;
  if (!m_unused.empty()) {
    ShadowNode *node = m_unused.back();
    m_unused.pop_back();

    node->setVertices(p1, p2);

    m_shadows.appendChild(node);

  } else {
    ShadowNode *node = new ShadowNode(p1, p2);
    m_shadows.appendChild(node);
  }
}

ShadowNode::Vertex::Vertex(float x, float y, float z, float w)
    : x(x), y(y), z(z), w(w) {}
