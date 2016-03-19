#include "StaticLight.hpp"
#include "LightSystem.hpp"
#include "QBox2D/Fixture/Box2DBox.hpp"
#include "QBox2D/QWorld.hpp"
#include "Utility/DisplayItem.hpp"
#include "Utility/Utility.hpp"
#include <QJsonObject>

StaticLight::StaticLight(Item *parent)
    : Light(parent), m_dynamicShadows(), m_dynamicLight() {
  addFixture(&m_fixture);

  m_fixture.setSensor(true);
  m_fixture.setVisible(false);
  m_fixture.setShadowCaster(false);
}

StaticLight::~StaticLight() { destroyBody(); }

void StaticLight::initialize(QWorld *w) {
  m_fixture.setPosition(QPointF(-radius(), -radius()));
  m_fixture.setSize(QSizeF(2 * radius(), 2 * radius()));

  assert(lightSystem());
  lightSystem()->addLight(this);

  QBody::initialize(w);
}

void StaticLight::destroyBody() {
  m_fixture.destroyFixture();

  if (lightSystem())
    lightSystem()->removeLight(this);

  Light::destroyBody();
}

bool StaticLight::read(const QJsonObject &obj) {
  Light::read(obj);
  setDynamicShadows(obj["dynamicShadows"].toBool());

  return true;
}

bool StaticLight::write(QJsonObject &obj) const {
  Light::write(obj);
  obj["dynamicShadows"] = dynamicShadows();

  return true;
}

void StaticLight::synchronize() {
  setTransform(position(), rotation());
  if (dynamicLight()) {
    dynamicLight()->setPosition(position());
  }
}

void StaticLight::visibleChanged() {
  if (lightSystem()) {
    lightSystem()->lightVisibilityChanged(this);
  }
}
