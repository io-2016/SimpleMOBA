#include "QJoint.hpp"
#include "QBody.hpp"
#include "QWorld.hpp"

QJoint::QJoint(QBody* b1, QBody* b2)
    : m_joint(), m_jointDef(), m_world(b1->world()), m_bodyA(b1), m_bodyB(b2) {}

QJoint::~QJoint() { destroyJoint(); }

void QJoint::initialize() {
  assert(m_bodyA && m_bodyB && m_world && m_jointDef);

  m_jointDef->bodyA = m_bodyA->body();
  m_jointDef->bodyB = m_bodyB->body();
  m_jointDef->userData = this;
  m_joint = m_world->world()->CreateJoint(m_jointDef);
}

void QJoint::destroyJoint() {
  if (m_joint) {
    m_world->world()->DestroyJoint(m_joint);

    m_joint = nullptr;
    m_jointDef = nullptr;
    m_world = nullptr;
    m_bodyA = nullptr;
    m_bodyB = nullptr;
  }
}

QPointF QJoint::anchorA() const {
  b2Vec2 p = joint()->GetAnchorA();
  return QPointF(p.x, p.y);
}

QPointF QJoint::anchorB() const {
  b2Vec2 p = joint()->GetAnchorB();
  return QPointF(p.x, p.y);
}

QJoint* QJoint::toQJoint(b2Joint* joint) {
  return static_cast<QJoint*>(joint->GetUserData());
}

QMouseJoint::QMouseJoint(QBody* body)
    : QJoint(body->world()->groundBody(), body) {}

void QMouseJoint::initialize() {
  setJointDef(&m_jointDef);

  QJoint::initialize();
}

void QMouseJoint::setTarget(QPointF p) {
  if (joint())
    static_cast<b2MouseJoint*>(joint())->SetTarget(b2Vec2(p.x(), p.y()));
  m_jointDef.target = b2Vec2(p.x(), p.y());
}

void QMouseJoint::setMaxForce(qreal f) {
  if (joint()) static_cast<b2MouseJoint*>(joint())->SetMaxForce(f);
  m_jointDef.maxForce = f;
}
