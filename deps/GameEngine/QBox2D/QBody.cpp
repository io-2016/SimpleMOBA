#include "QBody.hpp"

#include "QFixture.hpp"
#include "QWorld.hpp"
#include "Utility/Factory.hpp"
#include "Utility/Utility.hpp"

#include <QJsonArray>
#include <QJsonObject>

QBody::QBody(SceneGraph::Item *parent)
    : BaseItem(parent), m_body(), m_world(), m_id(), m_pendingDestroy() {
  m_bodyDef.userData = this;
  setVisible(false);
}

QBody::~QBody() { destroyBody(); }

void QBody::destroyBody() {
  if (!body())
    return;
  world()->onBodyDestroyed(this);

  while (QFixture *f = firstFixture()) {
    f->destroyFixture();
    releaseResource(f);
  }

  body()->SetUserData(nullptr);
  body()->GetWorld()->DestroyBody(body());

  m_world = nullptr;
  m_body = nullptr;
  m_pendingDestroy = false;

  setVisible(false);
}

QPointF QBody::position() const {
  return QPointF(m_bodyDef.position.x, m_bodyDef.position.y);
}

void QBody::setPosition(QPointF p) {
  m_bodyDef.position = b2Vec2(p.x(), p.y());

  resetTransform();
  translate(position().x(), position().y());
  rotate(rotation() * 180.0 / M_PI, 0, 0, 1);
}

void QBody::setRotation(qreal r) {
  m_bodyDef.angle = r * M_PI / 180.0;

  resetTransform();
  translate(position().x(), position().y());
  rotate(rotation(), 0, 0, 1);
}

void QBody::removeFixture(QFixture *fixture) {
  m_fixtureList.remove(&fixture->m_node);
}

void QBody::setLinearDamping(qreal linearDamping) {
  m_bodyDef.linearDamping = linearDamping;
  if (body())
    body()->SetLinearDamping(linearDamping);
}

void QBody::setAngularDamping(qreal angularDamping) {
  m_bodyDef.angularDamping = angularDamping;
  if (body())
    body()->SetAngularDamping(angularDamping);
}

void QBody::setGravityScale(qreal gravityScale) {
  m_bodyDef.gravityScale = gravityScale;
  if (body())
    body()->SetGravityScale(gravityScale);
}

QBody::BodyType QBody::bodyType() const {
  return static_cast<BodyType>(m_bodyDef.type);
}

void QBody::setBodyType(BodyType type) {
  m_bodyDef.type = static_cast<b2BodyType>(type);
  if (body())
    body()->SetType(static_cast<b2BodyType>(type));
}

void QBody::setBullet(bool bullet) {
  m_bodyDef.bullet = bullet;
  if (body())
    body()->SetBullet(bullet);
}

void QBody::setSleepingAllowed(bool allowed) {
  m_bodyDef.allowSleep = allowed;
  if (body())
    body()->SetSleepingAllowed(allowed);
}

void QBody::setFixedRotation(bool fixedRotation) {
  m_bodyDef.fixedRotation = fixedRotation;
  if (body())
    body()->SetFixedRotation(fixedRotation);
}

void QBody::setActive(bool active) {
  m_bodyDef.active = active;
  if (body())
    body()->SetActive(active);
}

QPointF QBody::linearVelocity() const {
  return QPointF(m_bodyDef.linearVelocity.x, m_bodyDef.linearVelocity.y);
}

void QBody::setLinearVelocity(const QPointF &linearVelocity) {
  b2Vec2 vec(linearVelocity.x(), linearVelocity.y());
  m_bodyDef.linearVelocity = vec;
  if (body())
    body()->SetLinearVelocity(vec);
}

void QBody::setAngularVelocity(qreal velocity) {
  m_bodyDef.angularVelocity = velocity;
  if (body())
    body()->SetAngularVelocity(velocity);
}

void QBody::addFixture(QFixture *f) {
  m_fixtureList.append(&f->m_node);
  f->setParent(this);
  if (body())
    f->initialize(this);
}

bool QBody::addFixture(const QJsonObject& obj, QFixture* fixture) {
  fixture->m_body = this;
  if (!fixture->read(obj))
    return false;
  addFixture(fixture);
  return true;
}

QFixture *QBody::firstFixture() const {
  return m_fixtureList.firstNode() ? m_fixtureList.firstNode()->data()
                                   : nullptr;
}

void QBody::setTransform(QPointF position, qreal rotation) {
  if (body()) {
    body()->SetTransform(b2Vec2(position.x(), position.y()), rotation);
  }
}

void QBody::initialize(QWorld *w) {
  if (body())
    return;

  m_world = w;
  m_body = world()->world()->CreateBody(&m_bodyDef);

  for (QFixture *f = firstFixture(); f; f = f->next())
    f->initialize(this);

  w->onBodyAdded(this);
}

void QBody::enqueueFunction(std::function<void()> f) {
  assert(world());

  if (m_work.empty())
    world()->m_enqueued.push_back(this);

  m_work.push(f);
}

void QBody::initializeLater(QWorld *w) {
  if (body())
    return;

  if (m_work.empty())
    w->m_enqueued.push_back(this);

  m_work.push(std::bind(
      static_cast<void (QBody::*)(QWorld *)>(&QBody::initialize), this, w));
}

void QBody::destroyLater() {
  if (m_pendingDestroy || !body())
    return;
  m_pendingDestroy = true;

  assert(world());
  world()->m_destroyed.push_back(this);
}

bool QBody::initialize(const QJsonObject &obj, QWorld *world) {
  m_world = world;
  if (!read(obj))
    return false;
  initialize(world);
  return true;
}

bool QBody::read(const QJsonObject &obj) {
  setPosition(QPointF(obj["x"].toDouble(), obj["y"].toDouble()));
  setRotation(obj["rotation"].toDouble());
  setBodyType(static_cast<BodyType>(obj["bodyType"].toInt()));
  setLinearDamping(obj["linearDamping"].toDouble());
  setAngularDamping(obj["angularDamping"].toDouble());
  setGravityScale(obj["gravityScale"].toDouble());

  QJsonArray array = obj["fixtures"].toArray();
  for (int i = 0; i < array.size(); i++) {
    QJsonObject obj = array[i].toObject();
    QByteArray name = obj["class"].toString().toLocal8Bit();
    QFixture *fixture = world()->factory()->create<QFixture>(name);
    assert(fixture);
    if (!addFixture(obj, fixture)) {
      qDebug() << "Failed to add fixture of type" << fixture;
      exit(1);
    }
  }

  return true;
}

bool QBody::write(QJsonObject &obj) const {
  obj["class"] = QString("QBody");
  obj["x"] = position().x();
  obj["y"] = position().y();
  obj["rotation"] = rotation();
  obj["bodyType"] = bodyType();
  obj["linearDamping"] = linearDamping();
  obj["angularDamping"] = angularDamping();
  obj["gravityScale"] = gravityScale();

  QJsonArray array;
  for (QFixture *f = firstFixture(); f; f = f->next()) {
    QJsonObject object;

    if (f->write(object))
      array.append(object);
  }

  obj["fixtures"] = array;

  return true;
}

void QBody::synchronize() {
  assert(body());

  qreal newX = body()->GetPosition().x;
  qreal newY = body()->GetPosition().y;
  qreal newRotation = body()->GetAngle() * 180.0 / M_PI;

  setPosition(QPointF(newX, newY));
  setRotation(newRotation);

  m_content.resetTransform();
  m_content.translate(position().x(), position().y());
  m_content.rotate(rotation(), 0, 0, 1);
}

void QBody::fixtureAdded(QFixture *f) { world()->onFixtureAdded(f); }

void QBody::fixtureDestroyed(QFixture *f) { world()->onFixtureDestroyed(f); }

void QBody::releaseResource(QFixture *f) { delete f; }

QBody *QBody::toQBody(b2Body *body) {
  return static_cast<QBody *>(body->GetUserData());
}

const QBody *QBody::toQBody(const b2Body *body) {
  return static_cast<const QBody *>(body->GetUserData());
}

void QBody::beginContact(QFixture *, b2Contact *) {}

void QBody::endContact(QFixture *, b2Contact *) {}

void QBody::preSolve(QFixture *, b2Contact *, const b2Manifold *) {}

void QBody::postSolve(QFixture *, b2Contact *, const b2ContactImpulse *) {}

void QBody::visibleChanged() { m_content.setVisible(visible()); }

bool QBody::testPoint(const QPointF &point) const {
  for (const b2Fixture *f = body()->GetFixtureList(); f; f = f->GetNext())
    if (f->TestPoint(b2Vec2(point.x(), point.y())))
      return true;
  return false;
}

bool QBody::testOverlap(const QRectF &rect) const {
  b2PolygonShape polygon;
  polygon.SetAsBox(rect.width(), rect.height(),
                   b2Vec2(rect.center().x(), rect.center().y()), 0);
  b2Transform transform;
  transform.SetIdentity();

  const b2Fixture *fixture = body()->GetFixtureList();
  while (fixture) {
    if (b2TestOverlap(&polygon, 0, fixture->GetShape(), 0, transform,
                      body()->GetTransform()))
      return true;

    fixture = fixture->GetNext();
  }
  return false;
}

bool QBody::testOverlap(const QBody *other) const {
  QRectF box1 = boundingRect();
  QRectF box2 = other->boundingRect();
  if (!box1.intersects(box2))
    return false;

  const b2Fixture *f1 = body()->GetFixtureList();
  while (f1) {
    const b2Fixture *f2 = other->body()->GetFixtureList();
    while (f2) {
      if (b2TestOverlap(f1->GetShape(), 0, f2->GetShape(), 0,
                        body()->GetTransform(), other->body()->GetTransform()))
        return true;

      f2 = f2->GetNext();
    }

    f1 = f1->GetNext();
  }

  return false;
}

void QBody::applyLinearImpulse(const QPointF &impulse, const QPointF &point) {
  if (body()) {
    body()->ApplyLinearImpulse(b2Vec2(impulse.x(), impulse.y()),
                               b2Vec2(point.x(), point.y()), true);
  }
}

void QBody::applyTorque(qreal torque) {
  if (body())
    body()->ApplyTorque(torque, true);
}

void QBody::applyForce(const QPointF &force) {
  if (body())
    body()->ApplyForceToCenter(b2Vec2(force.x(), force.y()), true);
}

QPointF QBody::worldCenter() const {
  QPointF worldCenter;

  if (body()) {
    const b2Vec2 &center = body()->GetWorldCenter();
    worldCenter.setX(center.x);
    worldCenter.setY(center.y);
  }

  return worldCenter;
}

QRectF QBody::boundingRect() const {
  b2AABB aabb;
  aabb.lowerBound = b2Vec2(FLT_MAX, FLT_MAX);
  aabb.upperBound = b2Vec2(-FLT_MAX, -FLT_MAX);

  assert(body());
  const b2Fixture *fixture = body()->GetFixtureList();
  while (fixture) {
    for (int i = 0; i < fixture->GetShape()->GetChildCount(); i++)
      aabb.Combine(fixture->GetAABB(i));
    fixture = fixture->GetNext();
  }

  return QRectF(QPointF(aabb.lowerBound.x, aabb.lowerBound.y),
                QPointF(aabb.upperBound.x, aabb.upperBound.y));
}
