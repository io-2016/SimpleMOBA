#include "AddBody.hpp"
#include "AddPolygon.hpp"
#include "AddCircle.hpp"
#include "Graphics/Primitives.hpp"
#include "QBox2D/QBody.hpp"
#include "QBox2D/QFixture.hpp"
#include "QBox2D/QWorld.hpp"
#include "MapEditor.hpp"

AddBody::AddBody(MapEditor* p)
    : SubAction(p),
      m_addPolygon(this),
      m_addCircle(this),
      m_addRectangle(this),
      m_currentAction(),
      m_action({addPolygon(), addCircle(), addRectangle()}),
      m_object(this) {}

void AddBody::finished() {
  QBody* body = new QBody(world());

  for (QFixture* f : m_fixtures) body->addFixture(f);
  body->initialize(world());

  world()->itemSet()->addBody(body);
  m_fixtures.clear();
}

void AddBody::subActionFinished(SubAction* action) {
  QFixture* fixture = static_cast<AddFixture*>(action)->fixture();
  if (fixture) {
    fixture->setParent(this);
    m_fixtures.push_back(fixture);
  }

  action->reset();
}

void AddBody::reset() {
  Action::reset();

  for (QFixture* f : m_fixtures) delete f;
  m_fixtures.clear();
}
