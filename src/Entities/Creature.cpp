#include "Creature.hpp"
#include "QBox2D/QWorld.hpp"
#include "QBox2D/QFixture.hpp"
#include <QJsonObject>
const float FORCE = 150;

Creature::Creature(SceneGraph::Item* parent)
    : QBody(parent), m_state(), m_health(100) {
  // setAcceptedMouseButtons(Qt::LeftButton);
}

void Creature::currentStateChanged(unsigned added, unsigned removed) {
  if (added & GoingLeft) addForce(QPointF(-FORCE, 0));
  if (added & GoingRight) addForce(QPointF(FORCE, 0));

  if (removed & GoingLeft) addForce(QPointF(FORCE, 0));
  if (removed & GoingRight) addForce(QPointF(-FORCE, 0));
}

void Creature::onWorldStepped() {
  updateOnGround();
  applyForce(force());

  enqueueFunction(std::bind(&Creature::onWorldStepped, this));
}

void Creature::addForce(const QPointF& force) { setForce(m_force + force); }

void Creature::modifyState(unsigned enable, unsigned disable) {
  enableState(enable);
  disableState(disable);
}

void Creature::enableState(unsigned state) {
  if ((currentState() | state) != currentState()) {
    unsigned mask = currentState();
    m_state |= state;
    currentStateChanged(mask ^ currentState(), 0);
  }
}

void Creature::disableState(unsigned state) {
  if (unsigned mask = currentState() & state) {
    m_state &= ~state;

    currentStateChanged(0, mask);
  }
}

void Creature::setHealth(int hp) {
  if (m_health == hp) return;
  m_health = hp;

  healthChanged();
}

bool Creature::read(const QJsonObject& obj) {
  QBody::read(obj);
  setHealth(obj["health"].toDouble());
  return true;
}

bool Creature::write(QJsonObject& obj) const {
  QBody::write(obj);
  obj["health"] = health();
  return true;
}

bool Creature::canSee(QBody* body) {
  RayCastCallback callback;
  callback.m_hit = nullptr;

  world()->rayCast(&callback, worldCenter(), body->worldCenter());
  return body == callback.m_hit;
}

void Creature::updateOnGround() {
  QRectF box = boundingRect();

  const qreal offset = 0.5;
  box.translate(0, offset);

  box = box.intersected(boundingRect());
  box.setWidth(box.width() - 2 * offset);
  box.translate(offset, 0);

  bool success = false;
  for (QFixture* f : world()->fixtures(box))
    if (this != f->body() && !f->isSensor()) {
      for (QFixture* tf = firstFixture(); tf; tf = tf->next()) {
        int16 g1 = tf->fixture()->GetFilterData().groupIndex;
        int16 g2 = f->fixture()->GetFilterData().groupIndex;

        if (!(g1 & g2) && !tf->fixture()->IsSensor()) {
          if (f->testOverlap(tf)) {
            success = true;
            break;
          }
        }
      }

      if (success) break;
    }

  if (success)
    enableState(OnGround);
  else
    disableState(OnGround);
}

void Creature::enableGoLeft() {
  modifyState(GoingLeft | TurnedLeft, TurnedRight);
}

void Creature::enableGoRight() {
  modifyState(GoingRight | TurnedRight, TurnedLeft);
}

void Creature::disableGoLeft() { disableState(GoingLeft); }

void Creature::disableGoRight() { disableState(GoingRight); }

void Creature::jumpRequested() {}

void Creature::punchRequested() {}

void Creature::jump() {
  if (currentState() & OnGround) applyForce(QPointF(0, -50 * FORCE));
}

void Creature::punch() {
  for (b2ContactEdge* c = body()->GetContactList(); c; c = c->next) {
    if (c->contact->GetFixtureA()->IsSensor()) continue;
    if (c->contact->GetFixtureB()->IsSensor()) continue;

    b2WorldManifold manifold;
    c->contact->GetWorldManifold(&manifold);
    QBody* body = QBody::toQBody(c->other);

    b2Vec2 normal = manifold.normal;
    QPointF v1((currentState() & TurnedLeft) ? QPointF(-1, 0) : QPointF(1, 0));
    QPointF v2(normal.x, normal.y);

    if (QPointF::dotProduct(v1, v2) < 0) {
      body->applyForce(-50000 * v2);
    }
  }
}

void Creature::initialize(QWorld* w) {
  QBody::initialize(w);
  onWorldStepped();
}

float32 Creature::RayCastCallback::ReportFixture(b2Fixture* fixture,
                                                 const b2Vec2&, const b2Vec2&,
                                                 float32 fraction) {
  m_hit = QFixture::toQFixture(fixture)->body();
  return fraction;
}
