#include "Action.hpp"
#include "Utility/Utility.hpp"
#include "SubAction.hpp"

Action::Action(SceneGraph::Item* parent, QWorld* w)
    : SceneGraph::Item(parent),
      m_currentSubAction(),
      m_enabled(),
      m_object(),
      m_world(w) {}

void Action::reset() {}

void Action::setEnabled(bool e) {
  if (m_enabled != e) {
    m_enabled = e;
    enabledChanged();
  }
}

void Action::enabledChanged() {
  if (m_object) emit m_object->enabledChanged();

  if (!enabled()) {
    if (m_currentSubAction) m_currentSubAction->setEnabled(false);
    setFocus(false);
    reset();
  } else {
    setFocus(true);
    onInit();
  }
}

void Action::subActionFinished(SubAction*) {}

void Action::subActionEnabledChanged(SubAction* action) {
  if (action->enabled()) {
    if (m_currentSubAction) m_currentSubAction->setEnabled(false);
    m_currentSubAction = action;
    action->setFocus(true);
  } else {
    if (m_currentSubAction == action) {
      m_currentSubAction = nullptr;
      setFocus(enabled());
    }
  }

  if (m_object) emit m_object->currentActionChanged();
}

ActionObject::ActionObject(Action* action) : m_action(action) {
  m_action->m_object = this;
}

QString ActionObject::currentAction() const {
  return m_action->m_currentSubAction ? m_action->m_currentSubAction->name()
                                      : "";
}

bool ActionObject::enabled() const { return m_action->enabled(); }

void ActionObject::toggleEnabled() {
  m_action->setEnabled(!m_action->enabled());
}

void ActionObject::finished() { m_action->finished(); }
