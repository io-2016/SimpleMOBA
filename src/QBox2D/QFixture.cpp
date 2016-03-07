#include "QFixture.hpp"
#include "QWorld.hpp"
#include "QBody.hpp"
#include "Utility/Utility.hpp"
#include <QJsonObject>

QFixture::QFixture(Item* parent)
    : SceneGraph::Item(parent),
      m_body(),
      m_fixture(),
      m_node(this),
      m_shadowCaster(true) {}

QFixture::~QFixture() { destroyFixture(); }

void QFixture::destroyFixture() {
  if (body()) {
    body()->fixtureDestroyed(this);
    body()->removeFixture(this);

    m_body = nullptr;
  }

  if (m_fixture) {
    m_fixture->SetUserData(nullptr);
    m_fixture->GetBody()->DestroyFixture(m_fixture);

    m_fixture = nullptr;
  }
}

void QFixture::setPosition(QPointF p) {
  m_position = p;

  resetTransform();
  translate(p.x(), p.y());
}

void QFixture::setDensity(float density) {
  m_fixtureDef.density = density;
  if (m_fixture) m_fixture->SetDensity(density);
}

void QFixture::setFriction(float friction) {
  m_fixtureDef.friction = friction;
  if (m_fixture) m_fixture->SetFriction(friction);
}

void QFixture::setRestitution(float restitution) {
  m_fixtureDef.restitution = restitution;
  if (m_fixture) m_fixture->SetRestitution(restitution);
}

void QFixture::setSensor(bool sensor) {
  m_fixtureDef.isSensor = sensor;
  if (m_fixture) m_fixture->SetSensor(sensor);
}

QFixture::CategoryFlag QFixture::categories() const {
  return CategoryFlag(m_fixtureDef.filter.categoryBits);
}

void QFixture::setCategories(CategoryFlag layers) {
  m_fixtureDef.filter.categoryBits = layers;
  if (m_fixture) m_fixture->SetFilterData(m_fixtureDef.filter);
}

QFixture::CategoryFlag QFixture::collidesWith() const {
  return CategoryFlag(m_fixtureDef.filter.maskBits);
}

void QFixture::setCollidesWith(CategoryFlag layers) {
  m_fixtureDef.filter.maskBits = layers;
  if (m_fixture) m_fixture->SetFilterData(m_fixtureDef.filter);
}

void QFixture::setGroupIndex(int groupIndex) {
  m_fixtureDef.filter.groupIndex = groupIndex;
  if (m_fixture) m_fixture->SetFilterData(m_fixtureDef.filter);
}

void QFixture::setShadowCaster(bool value) { m_shadowCaster = value; }

bool QFixture::write(QJsonObject& obj) const {
  obj["x"] = position().x();
  obj["y"] = position().y();
  obj["density"] = density();
  obj["friction"] = friction();
  obj["restitution"] = restitution();
  obj["shadowCaster"] = shadowCaster();
  return true;
}

QFixture* QFixture::next() const {
  return m_node.next() ? m_node.next()->data() : nullptr;
}

bool QFixture::testOverlap(QFixture* f) {
  return b2TestOverlap(fixture()->GetShape(), 0, f->fixture()->GetShape(), 0,
                       body()->body()->GetTransform(),
                       f->body()->body()->GetTransform());
}

bool QFixture::read(const QJsonObject& obj) {
  setPosition(QPointF(obj["x"].toDouble(), obj["y"].toDouble()));
  setDensity(obj["density"].toDouble());
  setFriction(obj["friction"].toDouble());
  setRestitution(obj["resititution"].toDouble());
  setShadowCaster(obj["shadowCaster"].toBool());
  return true;
}

void QFixture::initialize(QBody* item) {
  m_body = item;

  b2Shape* shape = createShape();

  m_fixtureDef.shape = shape;
  m_fixture = item->body()->CreateFixture(&m_fixtureDef);
  m_fixture->SetUserData(this);

  delete shape;

  m_content.setParent(item->content());

  item->fixtureAdded(this);
}

void QFixture::matrixChanged() { m_content.setMatrix(matrix()); }

QFixture* QFixture::toQFixture(b2Fixture* fixture) {
  return static_cast<QFixture*>(fixture->GetUserData());
}

const QFixture* QFixture::toQFixture(const b2Fixture* fixture) {
  return static_cast<const QFixture*>(fixture->GetUserData());
}
