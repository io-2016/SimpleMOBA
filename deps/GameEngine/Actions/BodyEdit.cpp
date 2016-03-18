#include "BodyEdit.hpp"
#include "QBox2D/QWorld.hpp"
#include "QBox2D/QBody.hpp"
#include "MapEditor.hpp"
#include "Utility/Utility.hpp"

BodyEdit::BodyEdit(MapEditor* p)
    : SubAction(p),
      m_body(),
      m_density(),
      m_friction(),
      m_restitution(),
      m_gravityScale(),
      m_linearDamping(),
      m_angularDamping(),
      m_dynamic(),
      m_object(this) {}

void BodyEdit::mousePressEvent(QMouseEvent* e) { e->ignore(); }

void BodyEdit::mouseReleaseEvent(QMouseEvent* event) {
  QBody* body = world()->bodyUnderPoint(mapFromScreen(event->pos()));

  if (body) {
    b2Fixture* fixture = body->body()->GetFixtureList();
    m_density = fixture->GetDensity();
    m_friction = fixture->GetFriction();
    m_restitution = fixture->GetRestitution();
    m_gravityScale = body->gravityScale();
    m_linearDamping = body->linearDamping();
    m_angularDamping = body->angularDamping();
    m_dynamic = body->bodyType() == QBody::Dynamic;
    emit m_object.update();
  }

  if (body != m_body) {
    m_body = body;
    emit m_object.bodyChanged();
  }

  event->ignore();
}

void BodyEdit::reset() {
  Action::reset();

  m_body = nullptr;
  emit m_object.bodyChanged();
}

void BodyEdit::applyChanges() const {
  if (!m_body) return;

  for (b2Fixture* f = m_body->body()->GetFixtureList(); f; f = f->GetNext()) {
    f->SetDensity(m_density);
    f->SetFriction(m_friction);
    f->SetRestitution(m_restitution);
  }

  m_body->setGravityScale(m_gravityScale);
  m_body->setLinearDamping(m_linearDamping);
  m_body->setAngularDamping(m_angularDamping);

  if (m_dynamic)
    m_body->setBodyType(QBody::Dynamic);
  else
    m_body->setBodyType(QBody::Static);
}

BodyEditObject::BodyEditObject(BodyEdit* action) : ActionObject(action) {}

BodyEdit* BodyEditObject::action() const {
  return static_cast<BodyEdit*>(ActionObject::action());
}

bool BodyEditObject::body() const { return action()->m_body != nullptr; }

qreal BodyEditObject::density() const { return action()->m_density; }

void BodyEditObject::setDensity(qreal x) { action()->m_density = x; }

qreal BodyEditObject::friction() const { return action()->m_friction; }

void BodyEditObject::setFriction(qreal x) { action()->m_friction = x; }

qreal BodyEditObject::restitution() const { return action()->m_restitution; }

void BodyEditObject::setRestitution(qreal x) { action()->m_restitution = x; }

qreal BodyEditObject::gravityScale() const { return action()->m_gravityScale; }

void BodyEditObject::setGravityScale(qreal x) { action()->m_gravityScale = x; }

qreal BodyEditObject::linearDamping() const {
  return action()->m_linearDamping;
}

void BodyEditObject::setLinearDamping(qreal x) {
  action()->m_linearDamping = x;
}

qreal BodyEditObject::angularDamping() const {
  return action()->m_angularDamping;
}

void BodyEditObject::setAngularDamping(qreal x) {
  action()->m_angularDamping = x;
}

bool BodyEditObject::dynamic() const { return action()->m_dynamic; }

void BodyEditObject::setDynamic(bool x) { action()->m_dynamic = x; }

void BodyEditObject::applyChanges() { action()->applyChanges(); }
