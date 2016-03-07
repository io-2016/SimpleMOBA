#include "Gun.hpp"
#include "World.hpp"
#include <QJsonObject>

Gun::Gun(SceneGraph::Item* parent) : Weapon(parent) {}

bool Gun::write(QJsonObject& obj) const {
  Weapon::write(obj);
  obj["class"] = QString("Gun");

  return true;
}

void Gun::shoot() {
  if (bulletCount() <= 0) return;
  setBulletCount(bulletCount() - 1);

  Circle circle((Vector2d)(matrix() * effectiveShootPoint()), 0.5);

  Bullet* bullet = new Bullet(circle, world());
  bullet->initialize(world());

  static_cast<World*>(world())->itemSet()->addBody(bullet);

  float angle = body()->GetAngle();
  bullet->applyForce(QPointF(cos(angle) * 5000, sin(angle) * 5000));
}

Bullet::Bullet(Circle circle, Item* p) : QBody(p), m_circle(this) {
  setPosition(QPointF(circle.pos()));
  setLinearDamping(0);
  setBullet(true);
  setBodyType(Dynamic);

  m_circle.setShadowCaster(false);
  m_circle.setRadius(circle.radius());
  m_circle.setGroupIndex(-1);

  addFixture(&m_circle);
}

void Bullet::destroyBody() {
  m_circle.destroyFixture();
  QBody::destroyBody();
}

void Bullet::beginContact(QFixture* other, b2Contact*) {
  if (other->isSensor()) return;

  destroyLater();

  ParticleSystem* p = static_cast<World*>(world())->particleSystem();
  p->addExplosion(worldCenter(), 5, 0.1, 20);
}
