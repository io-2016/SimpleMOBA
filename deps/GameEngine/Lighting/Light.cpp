#include "Light.hpp"
#include "LightSystem.hpp"
#include <QJsonObject>

Light::Light(SceneGraph::Item *parent)
    : QBody(parent), m_attenuation(0.5, 0.01, 0.0025), m_radius(1),
      m_renderFraction(2.5), m_z(1.0), m_lightSystem() {}

void Light::setAttenuation(QVector3D att) {
  m_attenuation = att;
  update();
}

void Light::setRadius(qreal r) {
  m_radius = r;
  update();
}

void Light::setRenderFraction(qreal r) {
  m_renderFraction = r;
  update();
}

void Light::setZ(qreal z) {
  m_z = z;
  update();
}

void Light::setColor(QColor col) {
  m_color = col;
  update();
}

QRectF Light::boundingRect() const {
  QPointF vec(radius(), radius());
  return QRectF(-vec, vec);
}

QRectF Light::renderRect() const {
  QPointF vec(renderFraction() * radius(), renderFraction() * radius());
  return QRectF(-vec, vec);
}

bool Light::read(const QJsonObject &obj) {
  setPosition(QPointF(obj["x"].toDouble(), obj["y"].toDouble()));
  setZ(obj["z"].toDouble());
  setRadius(obj["radius"].toDouble());
  setRenderFraction(obj["renderFraction"].toDouble());

  QJsonObject attData = obj["attenuation"].toObject();
  QVector3D att;
  att.setX(attData["x"].toDouble());
  att.setY(attData["y"].toDouble());
  att.setZ(attData["z"].toDouble());
  setAttenuation(att);

  QJsonObject colData = obj["color"].toObject();
  QColor col;
  col.setRed(colData["r"].toDouble());
  col.setGreen(colData["g"].toDouble());
  col.setBlue(colData["b"].toDouble());
  setColor(col);

  return true;
}

bool Light::write(QJsonObject &obj) const {
  obj["x"] = position().x();
  obj["y"] = position().y();
  obj["z"] = z();
  obj["radius"] = radius();
  obj["renderFraction"] = renderFraction();

  QJsonObject attData;
  attData["x"] = attenuation().x();
  attData["y"] = attenuation().y();
  attData["z"] = attenuation().z();
  obj["attenuation"] = attData;

  QJsonObject colData;
  colData["r"] = color().red();
  colData["g"] = color().green();
  colData["b"] = color().blue();
  obj["color"] = colData;

  return true;
}

SceneGraph::Node *Light::synchronize(SceneGraph::Node *old) {
  Light::LightNode *node = static_cast<LightNode *>(old);
  if (!node)
    node = new LightNode;

  node->synchronize(this);

  return node;
}

Light::LightNode::LightNode() : m_geometry({{2, GL_FLOAT}}, 4, sizeof(Vertex)) {
  setGeometry(&m_geometry);
  setMaterial(&m_material);
}

void Light::LightNode::updateGeometry(float radius) {
  Vertex *array = geometry()->vertexData<Vertex>();
  array[0] = {-radius, -radius};
  array[1] = {radius, -radius};
  array[2] = {-radius, radius};
  array[3] = {radius, radius};

  m_geometry.updateVertexData();
}

void Light::LightNode::synchronize(Light *light) {
  assert(light->lightSystem());
  assert(light->lightSystem()->normalMap());

  material()->setColor(light->color());
  material()->setNormalMap(light->lightSystem()->normalMap()->shaderNode());
  material()->setAttenuation(light->attenuation());
  material()->setLightPosition(QVector3D(0, 0, light->z()));

  updateGeometry(light->renderFraction() * light->radius());
}
